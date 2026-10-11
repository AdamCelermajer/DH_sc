"""Executed inside Blender by the local MCP socket. Never edits source mesh data."""
import bpy, hashlib, json, math
from pathlib import Path
from mathutils import Vector
from bpy_extras.object_utils import world_to_camera_view

def fingerprint(obj):
    m=obj.data
    payload={'positions':[list(v.co) for v in m.vertices], 'faces':[list(p.vertices) for p in m.polygons],
             'uv':[[list(d.uv) for d in layer.data] for layer in m.uv_layers]}
    return hashlib.sha256(json.dumps(payload,sort_keys=True).encode()).hexdigest()

def shape_fingerprint(obj):
    payload={'positions':[list(v.co) for v in obj.data.vertices],
             'faces':[(list(p.vertices),p.use_smooth) for p in obj.data.polygons]}
    return hashlib.sha256(json.dumps(payload,sort_keys=True).encode()).hexdigest()

def checkpoint(scene,message):
    bpy.context.window.scene=scene
    bpy.context.view_layer.update()
    bpy.ops.ed.undo_push(message=message)

def load(path):
    return bpy.data.images.load(str(Path(path).resolve()),check_existing=True)

def capture(cfg,out):
    name='TextureLab '+cfg['name']
    if (out/'capture.json').exists():raise ValueError('Output already contains a captured run; choose a new output directory')
    if bpy.data.scenes.get(name):raise ValueError('Scene already exists; use a fresh run name to preserve prior work')
    scene=bpy.data.scenes.new(name)
    originals=[]
    if cfg.get('source_obj'):
        checkpoint(scene,'Texture lab isolated OBJ import')
        bpy.ops.wm.obj_import(filepath=cfg['source_obj'],forward_axis='NEGATIVE_Z',up_axis='Y')
        originals=[o for o in scene.objects if o.type=='MESH']
        if cfg.get('obj_include'):
            for o in list(originals):
                if not any(o.name==n or o.name.startswith(n+'.') for n in cfg['obj_include']):
                    originals.remove(o);bpy.data.objects.remove(o,do_unlink=True)
            if not originals:raise ValueError('OBJ selection contains no matching meshes')
        mat=bpy.data.materials.new(name+' original atlas');mat.use_nodes=True
        tex=mat.node_tree.nodes.new('ShaderNodeTexImage');tex.image=load(cfg['atlas'])
        mat.node_tree.links.new(tex.outputs['Color'],mat.node_tree.nodes['Principled BSDF'].inputs['Base Color'])
        for o in originals:
            o.rotation_euler=[math.radians(x) for x in cfg.get('obj_rotation_degrees',[0,0,0])]
            o.data.materials.clear();o.data.materials.append(mat)
        archive=bpy.data.scenes.new(name+' imported original')
        for o in originals:
            archive.collection.objects.link(o);scene.collection.objects.unlink(o)
            copy=o.copy();copy.data=o.data.copy();copy.name=name+' '+o.name;scene.collection.objects.link(copy)
            copy.data.materials.clear();copy.data.materials.append(mat.copy())
    else:
        source=bpy.data.scenes[cfg['source_scene']]
        originals=[source.objects[n] for n in cfg['objects']]
        for o in originals:
            copy=o.copy();copy.data=o.data.copy();copy.name=name+' '+o.name;scene.collection.objects.link(copy)
            copy.data.materials.clear()
            for m in o.data.materials:copy.data.materials.append(m.copy())
        if source.world:scene.world=source.world.copy()
        for o in source.objects:
            if o.type=='LIGHT':
                copy=o.copy();copy.data=o.data.copy();scene.collection.objects.link(copy)
    if not scene.world:
        scene.world=bpy.data.worlds.new(name+' world');scene.world.use_nodes=True
        scene.world.node_tree.nodes['Background'].inputs[0].default_value=(0.08,0.08,0.08,1)
    meshes=[o for o in scene.objects if o.type=='MESH']
    checkpoint(scene,'Texture lab compute original bounds')
    points=[o.matrix_world@Vector(c) for o in meshes for c in o.bound_box]
    low=Vector([min(p[i] for p in points) for i in range(3)]);high=Vector([max(p[i] for p in points) for i in range(3)])
    center=(low+high)/2;extent=max(high-low);distance=max(10,extent*4)
    if not any(o.type=='LIGHT' for o in scene.objects):
        for suffix,pos,energy in [('key',Vector((-1,-2,2)),900),('fill',Vector((2,1,1)),650)]:
            data=bpy.data.lights.new(name+suffix,'AREA');data.energy=energy*(extent/2)**2;data.shape='DISK';data.size=extent*2
            obj=bpy.data.objects.new(name+suffix,data);scene.collection.objects.link(obj);obj.location=center+pos*extent
            obj.rotation_euler=(center-obj.location).to_track_quat('-Z','Y').to_euler()
    scene.render.engine='CYCLES';scene.cycles.samples=cfg.get('samples',32);scene.view_settings.view_transform='Standard'
    scene.render.resolution_x=scene.render.resolution_y=cfg.get('resolution',1024);scene.render.resolution_percentage=100
    views=[]
    for i,angle in enumerate(cfg.get('view_angles',[0,90,180,270])):
        data=bpy.data.cameras.new(name+f' view {i}');cam=bpy.data.objects.new(data.name,data);scene.collection.objects.link(cam)
        yaw=math.radians(angle);direction=Vector((math.sin(yaw),-math.cos(yaw),cfg.get('view_elevation',0)))
        cam.location=center+direction.normalized()*distance;cam.rotation_euler=(center-cam.location).to_track_quat('-Z','Y').to_euler()
        data.type='ORTHO';data.ortho_scale=extent*cfg.get('framing',1.15);data.clip_end=distance*10;scene.camera=cam
        if cfg.get('camera_overrides'):
            spec=cfg['camera_overrides'][i];cam.location=spec['location']
            cam.rotation_euler=(Vector(spec['target'])-cam.location).to_track_quat('-Z','Y').to_euler();data.ortho_scale=spec['ortho_scale']
        scene.render.filepath=str(out/f'view-{i}-reference.png');checkpoint(scene,'Texture lab reference capture')
        bpy.ops.render.render(write_still=True,scene=scene.name)
        views.append({'camera':cam.name,'reference':scene.render.filepath,'angle':angle})
    sources={o.name:fingerprint(o) for o in originals}
    manifest={'schema':'dh2-texture-lab-v1','scene':scene.name,'views':views,'sources':sources,
              'meshes':[{ 'name':o.name,'source_name':originals[i].name,'fingerprint':fingerprint(o),
                         'shape_fingerprint':shape_fingerprint(o),
                         'triangles':sum(len(p.vertices)-2 for p in o.data.polygons)} for i,o in enumerate(meshes)]}
    manifest['input_files']={field:{'path':cfg[field],'sha256':hashlib.sha256(Path(cfg[field]).read_bytes()).hexdigest()} for field in ('atlas','source_obj') if cfg.get(field)}
    (out/'capture.json').write_text(json.dumps(manifest,indent=2))
    bpy.ops.wm.save_as_mainfile(filepath=str(out/'references.blend'))
    return manifest

def project(cfg,out):
    manifest=json.loads((out/'capture.json').read_text());enhanced=json.loads((out/'enhanced.json').read_text())
    scene=bpy.data.scenes[manifest['scene']]
    checkpoint(scene,'Texture lab evaluate captured scene visibility')
    for field,record in manifest.get('input_files',{}).items():
        if hashlib.sha256(Path(record['path']).read_bytes()).hexdigest()!=record['sha256']:raise ValueError('Input file changed since capture: '+field)
    for source,expected in manifest['sources'].items():
        if fingerprint(bpy.data.objects[source])!=expected:raise ValueError('Source geometry changed since capture: '+source)
    coverage={}
    for entry in manifest['meshes']:
        obj=scene.objects[entry['name']];mesh=obj.data
        if fingerprint(obj)!=entry['fingerprint']:raise ValueError('Trial changed since capture; create a new run')
        if any(entry['source_name']==n or entry['source_name'].startswith(n+'.') for n in cfg.get('protect_objects',[])):continue
        original_uv=mesh.uv_layers.active.name
        valid=[]
        for i,view in enumerate(manifest['views']):
            camera=scene.objects[view['camera']];uv=mesh.uv_layers.new(name=f'TextureLabView{i}')
            for loop in mesh.loops:
                point=world_to_camera_view(scene,camera,obj.matrix_world@mesh.vertices[loop.vertex_index].co)
                uv.data[loop.index].uv=(point.x,point.y)
            attr=mesh.color_attributes.new(name=f'TextureLabWeight{i}',type='FLOAT_COLOR',domain='CORNER')
            camera_direction=(camera.matrix_world.to_quaternion()@Vector((0,0,1))).normalized()
            weights=[]
            depsgraph=bpy.context.evaluated_depsgraph_get()
            for p in mesh.polygons:
                normal=(obj.matrix_world.to_3x3().inverted().transposed()@p.normal).normalized()
                facing=max(0,normal.dot(camera_direction))**cfg.get('view_weight_power',4)
                center=obj.matrix_world@p.center
                origin=center+camera_direction*max(1,camera.data.ortho_scale*3)
                hit,where,_,_,hit_obj,_=scene.ray_cast(depsgraph,origin,-camera_direction)
                # Reject projections onto surfaces hidden behind another surface.
                visible=hit and hit_obj.original==obj and (where-center).length<camera.data.ortho_scale*0.015
                weights.append(facing if visible else 0)
            valid.append((uv,attr,weights,view,enhanced['views'][i]['path']))
        for p in mesh.polygons:
            total=sum(v[2][p.index] for v in valid)
            for _,attr,weights,_,_ in valid:
                weight=weights[p.index]/total if total>0 else 0
                for loopindex in p.loop_indices:attr.data[loopindex].color=(weight,weight,weight,1)
        coverage[obj.name]=sum(any(v[2][p.index]>0 for v in valid) for p in mesh.polygons)
        if coverage[obj.name]==0:raise ValueError('No visible projected polygons: '+obj.name)
        mesh.uv_layers.active=mesh.uv_layers[original_uv]
        for mat in mesh.materials:
            nodes=mat.node_tree.nodes;links=mat.node_tree.links
            bsdf=next(n for n in nodes if n.type=='BSDF_PRINCIPLED')
            base=bsdf.inputs['Base Color'].links[0].from_socket if bsdf.inputs['Base Color'].is_linked else None
            if base is None:
                rgb=nodes.new('ShaderNodeRGB');rgb.outputs[0].default_value=bsdf.inputs['Base Color'].default_value;base=rgb.outputs[0]
            # Weighted average of edited/reference ratios, with unobserved areas unchanged.
            total_ratio=None;total_weight=None
            for view_index,(uv,attr,_,view,path) in enumerate(valid):
                coords=nodes.new('ShaderNodeUVMap');coords.uv_map=uv.name
                textures=[]
                for image_path in (view['reference'],path):
                    tex=nodes.new('ShaderNodeTexImage');tex.image=load(image_path);tex.extension='CLIP';links.new(coords.outputs[0],tex.inputs['Vector']);textures.append(tex)
                ratio=nodes.new('ShaderNodeVectorMath');ratio.operation='DIVIDE';links.new(textures[1].outputs[0],ratio.inputs[0]);links.new(textures[0].outputs[0],ratio.inputs[1])
                low=nodes.new('ShaderNodeVectorMath');low.operation='MAXIMUM';low.inputs[1].default_value=(cfg.get('ratio_min',0.2),)*3;links.new(ratio.outputs[0],low.inputs[0])
                high=nodes.new('ShaderNodeVectorMath');high.operation='MINIMUM';high.inputs[1].default_value=(cfg.get('ratio_max',4),)*3;links.new(low.outputs[0],high.inputs[0])
                weight=nodes.new('ShaderNodeVertexColor');weight.layer_name=attr.name
                weight_socket=weight.outputs[0]
                regions=cfg.get('view_regions',{}).get(str(view_index),[])
                if regions:
                    separate=nodes.new('ShaderNodeSeparateXYZ');links.new(coords.outputs[0],separate.inputs[0]);region_mask=None
                    for x0,y0,x1,y1 in regions:
                        axes=[];feather=cfg.get('region_feather',0.01)
                        for axis,lo,hi in [('X',x0,x1),('Y',y0,y1)]:
                            a=nodes.new('ShaderNodeMapRange');a.clamp=True;links.new(separate.outputs[axis],a.inputs['Value']);a.inputs['From Min'].default_value=lo;a.inputs['From Max'].default_value=lo+feather
                            b=nodes.new('ShaderNodeMapRange');b.clamp=True;links.new(separate.outputs[axis],b.inputs['Value']);b.inputs['From Min'].default_value=hi-feather;b.inputs['From Max'].default_value=hi;b.inputs['To Min'].default_value=1;b.inputs['To Max'].default_value=0
                            multiply=nodes.new('ShaderNodeMath');multiply.operation='MULTIPLY';links.new(a.outputs[0],multiply.inputs[0]);links.new(b.outputs[0],multiply.inputs[1]);axes.append(multiply.outputs[0])
                        mask=nodes.new('ShaderNodeMath');mask.operation='MULTIPLY';links.new(axes[0],mask.inputs[0]);links.new(axes[1],mask.inputs[1])
                        if region_mask is None:region_mask=mask.outputs[0]
                        else:
                            union=nodes.new('ShaderNodeMath');union.operation='MAXIMUM';links.new(region_mask,union.inputs[0]);links.new(mask.outputs[0],union.inputs[1]);region_mask=union.outputs[0]
                    combine=nodes.new('ShaderNodeVectorMath');combine.operation='SCALE';links.new(weight_socket,combine.inputs[0]);links.new(region_mask,combine.inputs[3]);weight_socket=combine.outputs[0]
                weighted=nodes.new('ShaderNodeVectorMath');weighted.operation='MULTIPLY';links.new(high.outputs[0],weighted.inputs[0]);links.new(weight_socket,weighted.inputs[1])
                if total_ratio is None:total_ratio=weighted.outputs[0];total_weight=weight_socket
                else:
                    add=nodes.new('ShaderNodeVectorMath');add.operation='ADD';links.new(total_ratio,add.inputs[0]);links.new(weighted.outputs[0],add.inputs[1]);total_ratio=add.outputs[0]
                    addw=nodes.new('ShaderNodeVectorMath');addw.operation='ADD';links.new(total_weight,addw.inputs[0]);links.new(weight_socket,addw.inputs[1]);total_weight=addw.outputs[0]
            neutral=nodes.new('ShaderNodeVectorMath');neutral.operation='SUBTRACT';neutral.inputs[0].default_value=(1,1,1);links.new(total_weight,neutral.inputs[1])
            add=nodes.new('ShaderNodeVectorMath');add.operation='ADD';links.new(total_ratio,add.inputs[0]);links.new(neutral.outputs[0],add.inputs[1])
            mixed=nodes.new('ShaderNodeMixRGB');mixed.blend_type='MULTIPLY';mixed.inputs[0].default_value=cfg.get('strength',1);links.new(base,mixed.inputs[1]);links.new(add.outputs[0],mixed.inputs[2]);links.new(mixed.outputs[0],bsdf.inputs['Base Color'])
            mat['texture_lab_mask_node']=total_weight.node.name
            mat['texture_lab_mask_socket']=list(total_weight.node.outputs).index(total_weight)
    checkpoint(scene,'Texture lab multi-view projection')
    previews=[]
    for i,view in enumerate(manifest['views']):
        scene.camera=scene.objects[view['camera']];scene.render.filepath=str(out/f'view-{i}-projected.png')
        bpy.ops.render.render(write_still=True,scene=scene.name);previews.append(scene.render.filepath)
    bpy.ops.wm.save_as_mainfile(filepath=str(out/'projected.blend'))
    (out/'projection.json').write_text(json.dumps({'coverage_polygons':coverage,'previews':previews},indent=2))
    return {'previews':previews,'coverage_polygons':coverage,'triangles':sum(e['triangles'] for e in manifest['meshes'])}

def bake(cfg,out):
    manifest=json.loads((out/'capture.json').read_text());scene=bpy.data.scenes[manifest['scene']]
    meshes=[scene.objects[e['name']] for e in manifest['meshes']]
    atlas=bpy.data.images.load(cfg['atlas'],check_existing=False);atlas.name=cfg['name']+' baked atlas'
    size=cfg.get('atlas_size',list(atlas.size));atlas.scale(*size)
    baseline=atlas.copy();baseline.name=cfg['name']+' protected baseline'
    mask=bpy.data.images.new(cfg['name']+' edit coverage mask',width=size[0],height=size[1],alpha=False,float_buffer=True)
    mask.colorspace_settings.name='Non-Color'
    targets=[]
    for entry,obj in zip(manifest['meshes'],meshes):
        mesh=obj.data;source_uv=mesh.uv_layers.get(cfg.get('uv_map','UVMap'))
        if source_uv is None:raise ValueError('Required source UV map missing: '+cfg.get('uv_map','UVMap'))
        uv=mesh.uv_layers.new(name='TextureLabBakeUV');scale=cfg.get('uv_scale',{}).get(entry['source_name'],1)
        for loop in mesh.loops:uv.data[loop.index].uv=source_uv.data[loop.index].uv*scale
        for mat in mesh.materials:
            nodes=mat.node_tree.nodes;links=mat.node_tree.links;bsdf=next(n for n in nodes if n.type=='BSDF_PRINCIPLED')
            color=bsdf.inputs['Base Color'].links[0].from_socket
            emission=nodes.new('ShaderNodeEmission');links.new(color,emission.inputs[0]);output=next(n for n in nodes if n.type=='OUTPUT_MATERIAL');links.new(emission.outputs[0],output.inputs['Surface'])
            target=nodes.new('ShaderNodeTexImage');target.image=atlas
            for n in nodes:n.select=False
            target.select=True;nodes.active=target
            targets.append((mat,emission,target))
    scene.cycles.samples=1;scene.render.bake.margin=cfg.get('bake_margin',4);scene.render.bake.use_clear=False
    checkpoint(scene,'Texture lab bake original-layout atlas')
    for obj in scene.objects:obj.select_set(False)
    bake_objects=[];proxies=[]
    for entry,obj in zip(manifest['meshes'],meshes):
        side=cfg.get('canonical_bake_half',{}).get(entry['source_name'])
        if side:
            import bmesh
            proxy=obj.copy();proxy.data=obj.data.copy();scene.collection.objects.link(proxy)
            bm=bmesh.new();bm.from_mesh(proxy.data);axis=side.get('axis',0);sign=1 if side.get('keep','positive')=='positive' else -1
            bmesh.ops.delete(bm,geom=[f for f in bm.faces if f.calc_center_median()[axis]*sign<0],context='FACES')
            bm.to_mesh(proxy.data);bm.free();bake_objects.append(proxy);proxies.append(proxy)
        else:bake_objects.append(obj)
    for obj in bake_objects:obj.select_set(True)
    bpy.context.view_layer.objects.active=bake_objects[0];bpy.ops.object.bake(type='EMIT',uv_layer='TextureLabBakeUV',use_clear=False)
    for mat,emission,target in targets:
        target.image=mask
        links=mat.node_tree.links
        for link in list(emission.inputs[0].links):links.remove(link)
        if mat.get('texture_lab_mask_node'):
            node=mat.node_tree.nodes[mat['texture_lab_mask_node']]
            links.new(node.outputs[mat['texture_lab_mask_socket']],emission.inputs[0])
        else:emission.inputs[0].default_value=(0,0,0,1)
    bpy.ops.object.bake(type='EMIT',uv_layer='TextureLabBakeUV',use_clear=False)
    for proxy in proxies:bpy.data.objects.remove(proxy,do_unlink=True)
    atlas.filepath_raw=str(out/'atlas-raw-bake.png');atlas.file_format='PNG';atlas.save()
    mask.filepath_raw=str(out/'edit-mask.exr');mask.file_format='OPEN_EXR';mask.save()
    return finish(cfg,out)

def finish(cfg,out):
    manifest=json.loads((out/'capture.json').read_text());scene=bpy.data.scenes[manifest['scene']]
    meshes=[scene.objects[e['name']] for e in manifest['meshes']]
    atlas=load(out/'atlas-raw-bake.png');mask=load(out/'edit-mask.exr');size=list(atlas.size)
    baseline=bpy.data.images.load(cfg['atlas'],check_existing=False);baseline.scale(*size)
    composite=bpy.data.scenes.new(cfg['name']+' protected atlas composite');composite.camera=scene.camera
    composite.use_nodes=True
    composite.render.engine='CYCLES';composite.cycles.samples=1
    composite.render.resolution_x=size[0];composite.render.resolution_y=size[1];composite.render.resolution_percentage=100
    composite.view_settings.view_transform='Standard';composite.render.image_settings.color_mode='RGBA'
    tree=bpy.data.node_groups.new(cfg['name']+' atlas composite','CompositorNodeTree');composite.compositing_node_group=tree
    tree.interface.new_socket(name='Image',in_out='OUTPUT',socket_type='NodeSocketColor')
    nodes=tree.nodes;links=tree.links
    inputs=[]
    for image in (mask,baseline,atlas):
        node=nodes.new('CompositorNodeImage');node.image=image;inputs.append(node)
    mix=nodes.new('ShaderNodeMixRGB');links.new(inputs[0].outputs['Image'],mix.inputs[0]);links.new(inputs[1].outputs['Image'],mix.inputs[1]);links.new(inputs[2].outputs['Image'],mix.inputs[2])
    output=nodes.new('NodeGroupOutput');links.new(mix.outputs[0],output.inputs['Image'])
    composite.render.filepath=str(out/'atlas-candidate.png');checkpoint(composite,'Texture lab preserve pixels outside edit mask')
    bpy.ops.render.render(write_still=True,scene=composite.name)
    atlas=load(composite.render.filepath);atlas.pack()
    mat=bpy.data.materials.new(cfg['name']+' baked candidate');mat.use_nodes=True
    tex=mat.node_tree.nodes.new('ShaderNodeTexImage');tex.image=atlas;mat.node_tree.links.new(tex.outputs[0],mat.node_tree.nodes['Principled BSDF'].inputs['Base Color'])
    for obj in meshes:
        obj.data.materials.clear();obj.data.materials.append(mat);obj.data.uv_layers.active=obj.data.uv_layers['TextureLabBakeUV'];obj.data.uv_layers.active.active_render=True
    scene.cycles.samples=cfg.get('samples',32);checkpoint(scene,'Texture lab verify actual baked atlas')
    for i,view in enumerate(manifest['views']):
        scene.camera=scene.objects[view['camera']];scene.render.filepath=str(out/f'view-{i}-baked.png');bpy.ops.render.render(write_still=True,scene=scene.name)
    for name,expected in manifest['sources'].items():
        if fingerprint(bpy.data.objects[name])!=expected:raise ValueError('Source changed during bake')
    for entry,obj in zip(manifest['meshes'],meshes):
        if entry.get('shape_fingerprint') and shape_fingerprint(obj)!=entry['shape_fingerprint']:raise ValueError('Candidate shape changed during bake')
    bpy.ops.wm.save_as_mainfile(filepath=str(out/'candidate.blend'))
    result={'atlas':str(out/'atlas-candidate.png'),'dimensions':list(atlas.size),'original_geometry_unchanged':True,
            'triangles':sum(e['triangles'] for e in manifest['meshes']),'production_replaced':False,
            'limitations':['Shared/mirrored UV regions can receive competing paint; inspect all baked views.','No rig or game-runtime integration is performed.']}
    (out/'verification.json').write_text(json.dumps(result,indent=2));return result

def run(config_path,stage):
    config_path=Path(config_path).resolve();cfg=json.loads(config_path.read_text())
    for field in ('atlas','source_obj','output'):
        if cfg.get(field):cfg[field]=str((config_path.parent/Path(cfg[field])).resolve())
    out=Path(cfg['output']);out.mkdir(parents=True,exist_ok=True)
    return {'capture':capture,'project':project,'bake':bake,'finish':finish}[stage](cfg,out)
