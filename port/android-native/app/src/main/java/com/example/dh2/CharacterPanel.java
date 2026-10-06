package com.example.dh2;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.BitmapDrawable;
import android.text.Html;
import android.text.TextUtils;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.*;
import org.json.JSONArray;
import org.json.JSONObject;
import java.util.*;
/** Native Android presentation of the sole live character snapshot and actions. */
final class CharacterPanel extends FrameLayout {
    interface Host {
        void refresh(CharacterPanel panel);
        void action(CharacterPanel panel,int operation,int index,int slot);
        void close();
    }
    private static final int GOLD=Color.rgb(218,189,123),TEXT=Color.rgb(231,227,213),MUTED=Color.rgb(162,158,147),CARD=Color.rgb(29,32,36),BORDER=Color.rgb(88,76,52),RED=Color.rgb(222,143,130);
    private final Host host;
    private final LinearLayout tabs,body;
    private final TextView title,summary,message;
    private final Map<String,Bitmap> icons=new HashMap<>();
    private JSONObject snapshot;
    private int tab,selectedItem=-1,selectedSkill=-1,selectedFaery=-1,compareItem=-1,layoutWidth;
    private boolean busy,wide=true;
    private String report="";
    private int renderedTab;
    private final List<ScrollView> activeScrolls=new ArrayList<>();
    private final Map<String,Integer> scrollPositions=new HashMap<>();
    private final Set<ScrollView> pendingScrollRestore=Collections.newSetFromMap(new IdentityHashMap<ScrollView,Boolean>());
    CharacterPanel(Context context,Host host){
        super(context);
        this.host=host;
        setClickable(true);
        setFocusable(true);
        setElevation(dp(24));
        setBackground(new GradientDrawable(GradientDrawable.Orientation.TOP_BOTTOM,new int[]{
            Color.rgb(31,31,33),Color.rgb(17,20,24)
        }
        ));
        setContentDescription("Character stats equipment skills and faeries");
        LinearLayout shell=column();
        shell.setPadding(dp(12),dp(6),dp(12),dp(6));
        addView(shell,new FrameLayout.LayoutParams(-1,-1));
        LinearLayout header=row(),identity=column();
        title=text("Character",21,GOLD);
        title.setTypeface(Typeface.SERIF,Typeface.BOLD);
        identity.addView(title);
        summary=text("Loading character…",12,MUTED);
        identity.addView(summary);
        header.addView(identity,new LinearLayout.LayoutParams(0,-2,1));
        Button refresh=button("Refresh");
        refresh.setOnClickListener(v->{
            if(!busy)host.refresh(this);
        }
        );
        header.addView(refresh,new LinearLayout.LayoutParams(dp(80),dp(44)));
        Button close=button("Return to game");
        close.setContentDescription("Close character screen");
        close.setOnClickListener(v->host.close());
        header.addView(close,new LinearLayout.LayoutParams(dp(130),dp(44)));
        shell.addView(header);
        tabs=row();
        String[] names={
            "Stats","Equipment","Skills","Faeries"
        }
        ;
        for(int i=0;i<names.length;i++){
            final int n=i;
            Button b=button(names[i]);
            b.setContentDescription("Open "+names[i]);
            b.setOnClickListener(v->{
                tab=n;report="";render();
            }
            );
            LinearLayout.LayoutParams lp=new LinearLayout.LayoutParams(0,dp(44),1);
            lp.setMargins(dp(2),dp(4),dp(2),dp(5));
            tabs.addView(b,lp);
        }
        shell.addView(tabs);
        body=row();
        body.setGravity(Gravity.TOP);
        shell.addView(body,new LinearLayout.LayoutParams(-1,0,1));
        message=text("",12,MUTED);
        message.setPadding(dp(5),dp(3),dp(5),dp(2));
        message.setMaxLines(2);
        shell.addView(message,new LinearLayout.LayoutParams(-1,-2));
    }
    private int dp(float n){
        return Math.round(n*getResources().getDisplayMetrics().density);
    }
    private LinearLayout column(){
        LinearLayout x=new LinearLayout(getContext());
        x.setOrientation(LinearLayout.VERTICAL);
        return x;
    }
    private LinearLayout row(){
        LinearLayout x=new LinearLayout(getContext());
        x.setOrientation(LinearLayout.HORIZONTAL);
        x.setGravity(Gravity.CENTER_VERTICAL);
        return x;
    }
    private TextView text(String s,int size,int color){
        TextView x=new TextView(getContext());
        x.setText(s);
        x.setTextSize(size);
        x.setTextColor(color);
        x.setGravity(Gravity.CENTER_VERTICAL);
        return x;
    }
    private GradientDrawable frame(int fill,boolean selected){
        GradientDrawable d=new GradientDrawable();
        d.setColor(fill);
        d.setCornerRadius(dp(5));
        d.setStroke(dp(selected?2:1),selected?GOLD:BORDER);
        return d;
    }
    private Button button(String s){
        Button b=new Button(getContext());
        b.setText(s);
        b.setTextColor(TEXT);
        b.setTextSize(12);
        b.setAllCaps(false);
        b.setMinHeight(0);
        b.setMinimumHeight(0);
        b.setMinWidth(0);
        b.setMinimumWidth(0);
        b.setPadding(dp(7),dp(2),dp(7),dp(2));
        b.setBackground(frame(CARD,false));
        return b;
    }
    private LinearLayout card(){
        LinearLayout x=column();
        x.setPadding(dp(10),dp(8),dp(10),dp(8));
        x.setBackground(frame(CARD,false));
        return x;
    }
    private void attach(LinearLayout parent,View child){
        LinearLayout.LayoutParams lp=new LinearLayout.LayoutParams(-1,-2);
        lp.setMargins(dp(3),dp(3),dp(3),dp(5));
        parent.addView(child,lp);
    }
    private void heading(LinearLayout parent,String s){
        TextView t=text(s,16,GOLD);
        t.setTypeface(Typeface.SERIF,Typeface.BOLD);
        t.setPadding(0,dp(3),0,dp(6));
        parent.addView(t);
    }
    private void line(LinearLayout parent,String label,String value){
        LinearLayout r=row();
        TextView a=text(label,13,MUTED),b=text(value,14,TEXT);
        b.setGravity(Gravity.END|Gravity.CENTER_VERTICAL);
        r.addView(a,new LinearLayout.LayoutParams(0,dp(29),1));
        r.addView(b,new LinearLayout.LayoutParams(0,dp(29),1));
        parent.addView(r);
    }
    private void prose(LinearLayout parent,String html){
        if(html==null||html.isEmpty())return;
        TextView t=text("",13,TEXT);
        t.setText(Html.fromHtml(html.replace("\n","<br>"),Html.FROM_HTML_MODE_LEGACY));
        t.setPadding(0,dp(3),0,dp(7));
        t.setTextIsSelectable(true);
        parent.addView(t);
    }
    private void error(LinearLayout parent,JSONObject data){
        if(data.has("Error")){
            TextView t=text(data.optString("Error"),12,RED);
            t.setPadding(0,dp(4),0,dp(5));
            parent.addView(t);
        }
    }
    private static String value(JSONObject x,String key){
        Object v=x.opt(key);
        return v==null||v==JSONObject.NULL?"—":String.valueOf(v);
    }
    // Snapshot properties are already native signed fixed-point integers >>8.
    // This native presentation ratio never re-scales or rewrites those values.
    static final class ProgressFraction {
        static double ratio(double current,double maximum){
            if(Double.isNaN(current)||Double.isInfinite(current)||Double.isNaN(maximum)||Double.isInfinite(maximum)||maximum<=0)return Double.NaN;
            return Math.max(0,Math.min(1,current/maximum));
        }
    }
    private static String name(JSONObject x,String key,String fallback){
        String n=x.optString(key,"");
        return n.isEmpty()?fallback:n;
    }
    private static String plain(String s){
        return Html.fromHtml(s,Html.FROM_HTML_MODE_LEGACY).toString();
    }
    private static String slotLabel(String key){
        switch(key){
            case "RightHand":return "Right hand";
            case "LeftHand":return "Left hand";
            case "HandArmor":return "Hand armor";
            case "RightHandRingFinger":return "Right ring";
            case "LeftHandRingFinger":return "Left ring";
            default:return key;
        }
    }
    private String itemArt(JSONObject item){
        String original=item.optString("ItemIcon");
        if(!original.isEmpty())return original;
        String category=item.optString("ItemCategoryIcon");
        if(!category.isEmpty())return category;
        JSONArray categories=item.optJSONArray("ItemCategories");
        JSONObject first=categories==null?null:categories.optJSONObject(0);
        return first==null?"":first.optString("Icon");
    }
    private String itemCategories(JSONObject item){
        JSONArray categories=item.optJSONArray("ItemCategories");
        List<String> names=new ArrayList<>();
        if(categories!=null)for(int i=0;i<categories.length();i++){
            JSONObject category=categories.optJSONObject(i);
            if(category!=null){String label=category.optString("Name",category.optString("Icon"));if(!label.isEmpty())names.add(slotLabel(label));}
        }
        if(names.isEmpty()&&!item.optString("ItemCategoryName").isEmpty())return slotLabel(item.optString("ItemCategoryName"));
        return TextUtils.join(" / ",names);
    }
    private JSONObject find(JSONArray a,int id){
        if(a!=null)for(int i=0;i<a.length();i++){
            JSONObject x=a.optJSONObject(i);
            if(x!=null&&x.optInt("Index",-1)==id)return x;
        }
        return null;
    }
    private int first(JSONArray a){
        return a!=null&&a.length()>0&&a.optJSONObject(0)!=null?a.optJSONObject(0).optInt("Index",-1):-1;
    }
    private ScrollView scroll(LinearLayout content){
        ScrollView s=new ScrollView(getContext());
        s.setFillViewport(true);
        s.setClipToPadding(false);
        s.addView(content,new ScrollView.LayoutParams(-1,-2));
        int position=scrollPositions.containsKey(tab+":"+activeScrolls.size())?scrollPositions.get(tab+":"+activeScrolls.size()):0;
        activeScrolls.add(s);
        pendingScrollRestore.add(s);
        s.post(()->{
            if(s.getParent()!=null)s.scrollTo(0,position);pendingScrollRestore.remove(s);
        }
        );
        return s;
    }
    private LinearLayout[] columns(float weight){
        LinearLayout left=column(),right=column();
        if(wide){
            body.setOrientation(LinearLayout.HORIZONTAL);
            LinearLayout.LayoutParams a=new LinearLayout.LayoutParams(0,-1,weight),b=new LinearLayout.LayoutParams(0,-1,1-weight);
            a.setMargins(0,0,dp(5),0);
            body.addView(scroll(left),a);
            body.addView(scroll(right),b);
        }
        else{
            body.setOrientation(LinearLayout.VERTICAL);
            LinearLayout both=column();
            both.addView(left);
            both.addView(right);
            body.addView(scroll(both),new LinearLayout.LayoutParams(-1,-1));
        }
        return new LinearLayout[]{
            left,right
        }
        ;
    }
    @Override protected void onSizeChanged(int w,int h,int oldw,int oldh){
        super.onSizeChanged(w,h,oldw,oldh);
        if(w==layoutWidth)return;
        layoutWidth=w;
        wide=w>=dp(560);
        post(this::render);
    }
    private void command(int op,int index,int slot){
        if(busy)return;
        busy=true;
        report="Applying change…";
        render();
        host.action(this,op,index,slot);
    }
    void update(String json){
        busy=false;
        try{
            JSONObject next=new JSONObject(json);
            if(!next.optBoolean("ready")){
                snapshot=null;
                report=next.optString("error","Character unavailable");
            }
            else snapshot=next;
            render();
        }
        catch(Exception e){
            snapshot=null;
            report="Character data failed: "+e.getMessage();
            render();
        }
    }
    void result(String result){
        report=result;
        message.setText(result);
        message.setTextColor(result.toLowerCase(Locale.ROOT).contains("fail")?RED:TEXT);
    }
    void icons(Map<String,Bitmap> values){
        icons.putAll(values);
        render();
    }
    void publish(String json,Map<String,Bitmap> images,String result){
        busy=false;
        try{
            JSONObject next=new JSONObject(json);
            if(!next.optBoolean("ready")){
                snapshot=null;
                report=next.optString("error","Character unavailable");
            }
            else snapshot=next;
        }
        catch(Exception e){
            snapshot=null;
            report="Character data failed: "+e.getMessage();
        }
        if(images!=null)icons.putAll(images);
        if(result!=null)report=result;
        render();
    }
    Set<String> requestedIconNames(){
        Set<String> result=new LinkedHashSet<>(Arrays.asList("CharacterSheet","Inventory","Skills","Faery","MenuBack","MenuCompare"));
        if(snapshot==null)return result;
        String[] arrays={
            "items","skills","faeries","equipmentSlots"
        }
        ,keys={
            "ItemIcon","SkillIcon","Icon","Icon"
        }
        ;
        for(int n=0;n<arrays.length;n++){
            JSONArray a=snapshot.optJSONArray(arrays[n]);
            if(a==null)continue;
            for(int i=0;i<a.length();i++){
                JSONObject x=a.optJSONObject(i);
                if(x!=null){
                    String icon=x.optString(keys[n]);
                    if(!icon.isEmpty())result.add(icon);if(n==0){String category=x.optString("ItemCategoryIcon");if(!category.isEmpty())result.add(category);JSONArray entries=x.optJSONArray("ItemCategories");if(entries!=null)for(int j=0;j<entries.length();j++){JSONObject entry=entries.optJSONObject(j);if(entry!=null&&!entry.optString("Icon").isEmpty())result.add(entry.optString("Icon"));}}
                }
            }
        }
        return result;
    }
    private View icon(String key,String fallback,int size,boolean selected){
        FrameLayout f=new FrameLayout(getContext());
        f.setBackground(frame(Color.rgb(20,24,28),selected));
        Bitmap image=icons.get(key);
        if(image!=null&&!image.isRecycled()){
            ImageView v=new ImageView(getContext());
            v.setImageBitmap(image);
            v.setScaleType(ImageView.ScaleType.FIT_CENTER);
            v.setPadding(dp(4),dp(4),dp(4),dp(4));
            f.addView(v,new FrameLayout.LayoutParams(-1,-1));
        }
        else{
            TextView t=text(fallback,12,MUTED);
            t.setGravity(Gravity.CENTER);
            t.setMaxLines(2);
            f.addView(t,new FrameLayout.LayoutParams(-1,-1));
        }
        f.setLayoutParams(new LinearLayout.LayoutParams(dp(size),dp(size)));
        return f;
    }
    private void iconTitle(LinearLayout parent,JSONObject data,String nameKey,String iconKey,String fallback,String subtitle){
        LinearLayout r=row();
        r.addView(icon(iconKey.equals("ItemIcon")?itemArt(data):data.optString(iconKey),"",58,false));
        LinearLayout words=column();
        TextView n=text(plain(name(data,nameKey,fallback)),18,GOLD);
        n.setTypeface(Typeface.SERIF,Typeface.BOLD);
        words.addView(n);
        words.addView(text(subtitle,12,MUTED));
        LinearLayout.LayoutParams lp=new LinearLayout.LayoutParams(0,-2,1);
        lp.setMargins(dp(10),0,0,0);
        r.addView(words,lp);
        parent.addView(r);
    }
    private void progress(LinearLayout parent,JSONObject s,String label,String now,String max,int color){
        line(parent,label,value(s,now)+" / "+value(s,max));
        FrameLayout track=new FrameLayout(getContext());
        track.setBackground(frame(Color.rgb(12,15,18),false));
        View fill=new View(getContext());
        fill.setBackgroundColor(color);
        track.addView(fill,new FrameLayout.LayoutParams(0,-1));
        parent.addView(track,new LinearLayout.LayoutParams(-1,dp(8)));
        double n=s.optDouble(now,Double.NaN),m=s.optDouble(max,Double.NaN);
        double fraction=ProgressFraction.ratio(n,m);
        double ratio=Double.isNaN(fraction)?0:fraction;
        track.setContentDescription(Double.isNaN(fraction)?label+" progress unavailable":label+" "+value(s,now)+" of "+value(s,max));
        track.post(()->{
            if(fill.getParent()==track){
                FrameLayout.LayoutParams lp=(FrameLayout.LayoutParams)fill.getLayoutParams();lp.width=(int)Math.round(track.getWidth()*ratio);fill.setLayoutParams(lp);
            }
        }
        );
    }
    private void render(){
        for(int i=0;i<activeScrolls.size();i++){
            ScrollView old=activeScrolls.get(i);
            if(!pendingScrollRestore.contains(old))scrollPositions.put(renderedTab+":"+i,old.getScrollY());
        }
        activeScrolls.clear();
        renderedTab=tab;
        body.removeAllViews();
        for(int i=0;i<tabs.getChildCount();i++){
            Button b=(Button)tabs.getChildAt(i);
            b.setBackground(frame(i==tab?Color.rgb(71,59,39):CARD,i==tab));
            String[] art={
                "CharacterSheet","Inventory","Skills","Faery"
            }
            ;
            Bitmap bitmap=icons.get(art[i]);
            if(bitmap!=null&&!bitmap.isRecycled()){
                BitmapDrawable d=new BitmapDrawable(getResources(),bitmap);
                d.setBounds(0,0,dp(22),dp(22));
                b.setCompoundDrawablePadding(dp(6));
                b.setCompoundDrawables(d,null,null,null);
            }
        }
        message.setText(report.isEmpty()?"Changes apply to the current character.":report);
        message.setTextColor(busy?GOLD:MUTED);
        if(snapshot==null){
            LinearLayout p=card();
            heading(p,"Character unavailable");
            prose(p,report.isEmpty()?"Waiting for the current character…":report);
            body.addView(p,new LinearLayout.LayoutParams(-1,-2));
            return;
        }
        JSONObject s=snapshot.optJSONObject("stats");
        if(s==null){
            message.setText("Character stats unavailable");
            return;
        }
        String characterName=s.optString("Name");
        title.setText(characterName.isEmpty()?name(s,"Class","Character"):plain(characterName)+"  •  "+name(s,"Class","Character"));
        summary.setText("Level "+value(s,"Level")+"    Gold "+value(snapshot,"gold")+"    Skill points "+value(snapshot,"skillPoints")+"    Set "+(snapshot.optInt("equipmentSet")+1));
        if(tab==0)stats(s);
        else if(tab==1)equipment(snapshot.optJSONArray("items"));
        else if(tab==2)skills(snapshot.optJSONArray("skills"));
        else faeries(snapshot.optJSONArray("faeries"));
    }
    private void stats(JSONObject s){
        LinearLayout[] c=columns(.43f);
        LinearLayout vitals=card();
        heading(vitals,"Vital statistics");
        progress(vitals,s,"Health","HP","Max_HP",Color.rgb(146,53,48));
        progress(vitals,s,"Mana","MP","Max_MP",Color.rgb(58,98,160));
        progress(vitals,s,"Experience","XP","Max_XP",Color.rgb(160,131,60));
        attach(c[0],vitals);
        LinearLayout attrs=card();
        heading(attrs,"Attributes");
        prose(attrs,value(s,"Stat_Points")+" unspent points");
        String[] keys={
            "Stat_Strength","Stat_Dexterity","Stat_Endurance","Stat_Energy"
        }
        ,labels={
            "Strength","Dexterity","Endurance","Energy"
        }
        ;
        for(int i=0;i<4;i++){
            final int stat=i;
            LinearLayout r=row();
            r.addView(text(labels[i],14,TEXT),new LinearLayout.LayoutParams(0,dp(44),1));
            TextView v=text(value(s,keys[i]),16,GOLD);
            v.setGravity(Gravity.CENTER);
            r.addView(v,new LinearLayout.LayoutParams(dp(48),dp(44)));
            Button add=button("+ 1");
            add.setEnabled(!busy&&s.optInt("Stat_Points")>0);
            add.setContentDescription("Assign one "+labels[i]+" point");
            add.setOnClickListener(x->command(2,stat,-1));
            r.addView(add,new LinearLayout.LayoutParams(dp(52),dp(44)));
            attrs.addView(r);
        }
        attach(c[0],attrs);
        String[][] sections={
            {
                "Offense","Rating_Attack","Attack rating","Rating_Critical","Critical rating","Damage_Min_Main_Hand","Main hand minimum","Damage_Max_Main_Hand","Main hand maximum","Damage_Min_Off_Hand","Off hand minimum","Damage_Max_Off_Hand","Off hand maximum"
            }
            ,{
                "Defense","Rating_Defense","Defense rating","Rating_Dodge","Dodge rating","Rating_Block","Block rating","Physical_Armor","Armor"
            }
            ,{
                "Resistance","Resistance_Fire","Fire","Resistance_Water","Water","Resistance_Lightning","Lightning","Resistance_Earth","Earth","Resistance_Air","Air"
            }
            ,{
                "Magic","Spell_Rating_Critical","Spell critical rating","Spell_Rating_Dodge","Spell dodge rating","Menu_Average_Spell_To_Hit","Spell hit chance","Spell_Damage_Bonus_Fire","Fire damage bonus","Spell_Damage_Bonus_Water","Water damage bonus","Spell_Damage_Bonus_Lightning","Lightning damage bonus","Spell_Damage_Bonus_Earth","Earth damage bonus","Spell_Damage_Bonus_Air","Air damage bonus"},{"Recovery","Regen_HP","Health regeneration","Regen_MP","Mana regeneration","Leech_HP","Health leech","Leech_MP","Mana leech"
            }
        }
        ;
        for(String[] section:sections){
            LinearLayout p=card();
            heading(p,section[0]);
            for(int j=1;j<section.length;j+=2)line(p,section[j+1],value(s,section[j]));
            attach(c[1],p);
        }
    }
    private void equipment(JSONArray items){
        LinearLayout[] c=columns(.47f);
        if(items==null){
            prose(c[0],"Inventory unavailable");
            return;
        }
        if(find(items,selectedItem)==null)selectedItem=first(items);
        LinearLayout equipped=card();
        LinearLayout h=row();
        h.addView(text("Equipped · set "+(snapshot.optInt("equipmentSet")+1),16,GOLD),new LinearLayout.LayoutParams(0,-2,1));
        Button swap=button("Switch set");
        swap.setEnabled(!busy);
        swap.setOnClickListener(v->command(5,-1,-1));
        h.addView(swap,new LinearLayout.LayoutParams(dp(86),dp(44)));
        equipped.addView(h);
        JSONArray metadata=snapshot.optJSONArray("equipmentSlots");
        LinearLayout pair=null;
        for(int slot=0;slot<9;slot++){
            JSONObject meta=find(metadata,slot),item=meta==null?null:find(items,meta.optInt("ItemIndex",-1));
            if(meta==null)for(int i=0;i<items.length();i++){
                JSONObject x=items.optJSONObject(i);
                if(x!=null&&x.optInt("EquippedSlot",-1)==slot){
                    item=x;
                    break;
                }
            }
            if(slot%2==0){
                pair=row();
                equipped.addView(pair);
            }
            LinearLayout cell=column();
            cell.setPadding(dp(4),dp(4),dp(4),dp(4));
            cell.setBackground(frame(Color.rgb(22,25,29),item!=null&&item.optInt("Index")==selectedItem));
            String label=meta!=null?meta.optString("Name","Slot "+(slot+1)):item==null?"Slot "+(slot+1):item.optString("EquipmentSlotName","Slot "+(slot+1));
            label=slotLabel(label);TextView lbl=text(label,11,MUTED);
            lbl.setSingleLine(true);
            cell.addView(lbl);
            LinearLayout r=row();
            String art=item!=null&&!itemArt(item).isEmpty()?itemArt(item):meta==null?"":meta.optString("Icon");
            r.addView(icon(art,"—",36,false));
            TextView n=text(item==null?"Empty":plain(name(item,"ItemName","Item")),12,item==null?MUTED:TEXT);
            n.setMaxLines(2);
            r.addView(n,new LinearLayout.LayoutParams(0,dp(42),1));
            cell.addView(r);
            final JSONObject choice=item;
            if(choice!=null)cell.setOnClickListener(v->{
                selectedItem=choice.optInt("Index");render();
            }
            );
            cell.setContentDescription(label+", "+(item==null?"empty":plain(item.optString("ItemName"))));
            LinearLayout.LayoutParams lp=new LinearLayout.LayoutParams(0,-2,1);
            lp.setMargins(dp(2),dp(2),dp(2),dp(2));
            pair.addView(cell,lp);
            if(slot==8)pair.addView(new View(getContext()),new LinearLayout.LayoutParams(0,1,1));
        }
        attach(c[0],equipped);
        LinearLayout bag=card();
        heading(bag,"Inventory · "+items.length()+" items");
        if(items.length()==0)prose(bag,"Your inventory is empty.");
        int count=wide?3:4;
        LinearLayout gridRow=null;
        for(int i=0;i<items.length();i++){
            JSONObject item=items.optJSONObject(i);
            if(item==null)continue;
            if(i%count==0){
                gridRow=row();
                bag.addView(gridRow);
            }
            LinearLayout cell=column();
            cell.setGravity(Gravity.CENTER);
            cell.setPadding(dp(4),dp(4),dp(4),dp(4));
            int id=item.optInt("Index");
            cell.setBackground(frame(Color.rgb(21,24,28),id==selectedItem));
            cell.addView(icon(itemArt(item),"Item",48,id==selectedItem));
            TextView n=text(plain(name(item,"ItemName","Item")),11,TEXT);
            n.setGravity(Gravity.CENTER);
            n.setMaxLines(2);
            n.setEllipsize(TextUtils.TruncateAt.END);
            cell.addView(n,new LinearLayout.LayoutParams(-1,dp(32)));
            String marker=item.optInt("EquippedSlot",-1)>=0?"Equipped":"× "+value(item,"Quantity");
            TextView m=text(marker,10,GOLD);
            m.setGravity(Gravity.CENTER);
            cell.addView(m);
            cell.setContentDescription(plain(item.optString("ItemName"))+", "+marker);
            cell.setOnClickListener(v->{
                selectedItem=id;render();
            }
            );
            LinearLayout.LayoutParams lp=new LinearLayout.LayoutParams(0,dp(116),1);
            lp.setMargins(dp(2),dp(2),dp(2),dp(2));
            gridRow.addView(cell,lp);
        }
        if(gridRow!=null){
            int last=items.length()%count;
            if(last!=0)for(int i=last;i<count;i++)gridRow.addView(new View(getContext()),new LinearLayout.LayoutParams(0,1,1));
        }
        attach(c[0],bag);
        JSONObject selected=find(items,selectedItem);
        if(selected==null)return;
        LinearLayout detail=card();
        int slot=selected.optInt("EquippedSlot",-1);
        iconTitle(detail,selected,"ItemName","ItemIcon","Item",slot>=0?slotLabel(selected.optString("EquipmentSlotName","Equipped slot "+(slot+1))):"In inventory");
        itemDetails(detail,selected);
        Button equip=button(slot>=0?"Unequip item":"Equip item");
        equip.setEnabled(!busy&&(slot>=0||selected.optBoolean("ItemEquippable")));
        equip.setContentDescription((slot>=0?"Unequip ":"Equip ")+plain(selected.optString("ItemName")));
        equip.setOnClickListener(v->command(slot>=0?1:0,selected.optInt("Index"),slot));
        detail.addView(equip,new LinearLayout.LayoutParams(-1,dp(44)));
        if(slot<0&&!selected.optBoolean("ItemEquippable"))prose(detail,"This character cannot equip this item.");
        attach(c[1],detail);
        LinearLayout compare=card();
        heading(compare,"Compare equipped items");
        List<JSONObject> candidates=new ArrayList<>();
        List<String> labels=new ArrayList<>();
        labels.add("Choose equipped item");
        JSONArray compatible=selected.optJSONArray("CompatibleSlots");
        Set<Integer> matching=new HashSet<>();
        if(compatible!=null)for(int i=0;i<compatible.length();i++)matching.add(compatible.optInt(i,-1));
        for(int i=0;i<items.length();i++){
            JSONObject x=items.optJSONObject(i);
            if(x!=null&&x.optInt("EquippedSlot",-1)>=0&&x.optInt("Index")!=selectedItem&&(compatible==null||matching.contains(x.optInt("EquippedSlot")))){
                candidates.add(x);
                labels.add(x.optString("EquipmentSlotName","Slot "+(x.optInt("EquippedSlot")+1))+" · "+plain(x.optString("ItemName")));
            }
        }
        if(candidates.isEmpty())prose(compare,"No equipped item in a compatible slot.");
        else{
            boolean retained=false;
            for(JSONObject x:candidates)if(x.optInt("Index")==compareItem)retained=true;
            if(!retained&&compareItem>=0)compareItem=-1;
            Spinner picker=new Spinner(getContext());
            ArrayAdapter<String> adapter=new ArrayAdapter<String>(getContext(),android.R.layout.simple_spinner_dropdown_item,labels){
                private View label(int position){TextView t=text(getItem(position),13,TEXT);t.setPadding(dp(8),dp(8),dp(8),dp(8));t.setMinHeight(dp(44));t.setBackgroundColor(CARD);return t;}
                @Override public View getView(int position,View recycled,ViewGroup parent){return label(position);}
                @Override public View getDropDownView(int position,View recycled,ViewGroup parent){return label(position);}
            };
            picker.setAdapter(adapter);
            int choice=0;
            for(int i=0;i<candidates.size();i++)if(candidates.get(i).optInt("Index")==compareItem)choice=i+1;
            picker.setSelection(choice);
            compare.addView(picker,new LinearLayout.LayoutParams(-1,dp(44)));
            JSONObject compared=find(items,compareItem);
            if(compared!=null){
                prose(compare,"Equipped: "+compared.optString("ItemName"));
                itemDetails(compare,compared);
            }
            picker.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener(){
                public void onNothingSelected(AdapterView<?> p){
                }
                public void onItemSelected(AdapterView<?> p,View v,int at,long id){
                    int n=at==0?-1:candidates.get(at-1).optInt("Index");if(n!=compareItem){
                        compareItem=n;render();
                    }
                }
            }
            );
        }
        attach(c[1],compare);
    }
    private void itemDetails(LinearLayout p,JSONObject item){String categories=itemCategories(item);if(!categories.isEmpty())line(p,"Category",categories);
        prose(p,item.optString("ItemStatsDesc"));
        for(int i=0;i<item.optInt("ItemMagics");i++)prose(p,item.optString("ItemPowers"+i+"Desc"));
        prose(p,item.optString("ItemReqsDesc"));
        line(p,"Quantity",value(item,"Quantity"));
        line(p,"Value",value(item,"ItemValueString"));
        error(p,item);
    }
    private void skills(JSONArray skills){
        LinearLayout[] c=columns(.48f);
        if(skills==null){
            prose(c[0],"Skills unavailable");
            return;
        }
        if(find(skills,selectedSkill)==null)selectedSkill=first(skills);
        LinearLayout loadout=card();
        heading(loadout,"Active skill slots");
        LinearLayout slots=row();
        for(int i=0;i<3;i++){
            JSONObject skill=null;
            for(int j=0;j<skills.length();j++){
                JSONObject x=skills.optJSONObject(j);
                if(x!=null&&x.optInt("EquippedSlot",-1)==i){
                    skill=x;
                    break;
                }
            }
            LinearLayout cell=column();
            cell.setGravity(Gravity.CENTER);
            cell.addView(icon(skill==null?"":skill.optString("SkillIcon"),String.valueOf(i+1),44,false));
            TextView label=text("Slot "+(i+1),11,GOLD);
            label.setGravity(Gravity.CENTER);
            cell.addView(label);
            TextView n=text(skill==null?"Empty":plain(skill.optString("SkillName")),11,TEXT);
            n.setMaxLines(2);
            n.setGravity(Gravity.CENTER);
            cell.addView(n);
            final JSONObject choice=skill;
            if(choice!=null)cell.setOnClickListener(v->{
                selectedSkill=choice.optInt("Index");render();
            }
            );
            slots.addView(cell,new LinearLayout.LayoutParams(0,dp(95),1));
        }
        loadout.addView(slots);
        attach(c[0],loadout);
        LinearLayout tree=card();
        heading(tree,"Skills · "+value(snapshot,"skillPoints")+" points");
        for(int i=0;i<skills.length();i++){
            JSONObject skill=skills.optJSONObject(i);
            if(skill==null)continue;
            int id=skill.optInt("Index"),level=skill.optInt("SkillLevel");
            boolean unlocked=skill.optBoolean("SkillUnlocked");
            LinearLayout cell=row();
            cell.setPadding(dp(5),dp(5),dp(5),dp(5));
            cell.setBackground(frame(Color.rgb(21,24,28),id==selectedSkill));
            cell.addView(icon(skill.optString("SkillIcon"),"",48,id==selectedSkill));
            LinearLayout words=column();
            words.addView(text(plain(name(skill,"SkillName","Skill "+(id+1))),14,unlocked?TEXT:MUTED));
            String state=!skill.has("SkillUnlocked")?"Details unavailable":level>0?"Rank "+value(skill,"SkillLevel"):unlocked?"Available to learn":"Requires level "+value(skill,"RequiredLevel");
            if(skill.optInt("EquippedSlot",-1)>=0)state+=" · Slot "+(skill.optInt("EquippedSlot")+1);
            words.addView(text(state,11,unlocked?GOLD:MUTED));
            LinearLayout.LayoutParams lp=new LinearLayout.LayoutParams(0,-2,1);
            lp.setMargins(dp(8),0,0,0);
            cell.addView(words,lp);
            cell.setAlpha(unlocked||level>0?1f:.7f);
            cell.setContentDescription(plain(skill.optString("SkillName"))+", "+state);
            cell.setOnClickListener(v->{
                selectedSkill=id;render();
            }
            );
            attach(tree,cell);
        }
        attach(c[0],tree);
        JSONObject skill=find(skills,selectedSkill);
        if(skill==null)return;
        LinearLayout detail=card();
        int level=skill.optInt("SkillLevel");
        iconTitle(detail,skill,"SkillName","SkillIcon","Skill","Rank "+level+" · Requires character level "+value(skill,"RequiredLevel"));
        prose(detail,skill.optString("SkillDescription"));
        if(!skill.optString("SkillCurrLevel").isEmpty()){
            heading(detail,"Current rank");
            prose(detail,skill.optString("SkillCurrLevel"));
        }
        if(!skill.optString("SkillNextLevel").isEmpty()){
            heading(detail,"Next rank");
            prose(detail,skill.optString("SkillNextLevel"));
        }
        error(detail,skill);
        Button train=button(level==0?"Learn skill":"Train skill");
        train.setEnabled(!busy&&snapshot.optInt("skillPoints")>0&&skill.optBoolean("SkillUnlocked")&&!skill.has("Error"));
        train.setOnClickListener(v->command(3,skill.optInt("Index"),-1));
        detail.addView(train,new LinearLayout.LayoutParams(-1,dp(44)));
        if(!skill.has("SkillUnlocked"))prose(detail,"Skill availability is unavailable.");else if(!skill.optBoolean("SkillUnlocked"))prose(detail,"This skill is locked for the current character.");
        else if(snapshot.optInt("skillPoints")<=0)prose(detail,"No unspent skill points.");
        attach(c[1],detail);
        LinearLayout assign=card();
        heading(assign,"Assign to an active slot");
        if(level>0&&skill.optBoolean("SkillAssignable")){
            LinearLayout buttons=row();
            for(int i=0;i<3;i++){
                final int slot=i;
                Button b=button("Slot "+(i+1));
                b.setEnabled(!busy);
                boolean active=skill.optInt("EquippedSlot",-1)==i;
                b.setBackground(frame(active?Color.rgb(71,59,39):CARD,active));
                b.setOnClickListener(v->command(4,skill.optInt("Index"),slot));
                LinearLayout.LayoutParams lp=new LinearLayout.LayoutParams(0,dp(44),1);
                lp.setMargins(dp(2),0,dp(2),0);
                buttons.addView(b,lp);
            }
            assign.addView(buttons);
        }
        else prose(assign,level==0?"Learn this skill before assigning it.":"This skill cannot be assigned to an active slot.");
        attach(c[1],assign);
    }
    private void faeries(JSONArray faeries){
        LinearLayout[] c=columns(.43f);
        if(faeries==null||faeries.length()==0){
            prose(c[0],"Faery information unavailable");
            return;
        }
        if(find(faeries,selectedFaery)==null){
            selectedFaery=first(faeries);
            for(int i=0;i<faeries.length();i++){
                JSONObject x=faeries.optJSONObject(i);
                if(x!=null&&x.optBoolean("Selected"))selectedFaery=x.optInt("Index");
            }
        }
        LinearLayout roster=card();
        heading(roster,"Faery companions");
        for(int i=0;i<faeries.length();i++){
            JSONObject f=faeries.optJSONObject(i);
            if(f==null)continue;
            int id=f.optInt("Index");
            boolean locked=f.optInt("State",-1)!=1;
            LinearLayout cell=row();
            cell.setPadding(dp(6),dp(6),dp(6),dp(6));
            cell.setBackground(frame(Color.rgb(21,24,28),id==selectedFaery));
            cell.addView(icon(f.optString("Icon"),"",48,id==selectedFaery));
            LinearLayout words=column();
            words.addView(text(plain(name(f,"Name","Faery "+(id+1))),15,locked?MUTED:TEXT));
            words.addView(text(locked?"Locked":f.optBoolean("Selected")?"Active companion · Level "+value(f,"Level"):"Unlocked · Level "+value(f,"Level"),11,locked?MUTED:GOLD));
            LinearLayout.LayoutParams lp=new LinearLayout.LayoutParams(0,-2,1);
            lp.setMargins(dp(8),0,0,0);
            cell.addView(words,lp);
            cell.setOnClickListener(v->{
                selectedFaery=id;render();
            }
            );
            attach(roster,cell);
        }
        attach(c[0],roster);
        JSONObject f=find(faeries,selectedFaery);
        if(f==null)return;
        LinearLayout detail=card();
        boolean locked=f.optInt("State",-1)!=1;
        iconTitle(detail,f,"Name","Icon","Faery",locked?"Locked in this campaign":f.optBoolean("Selected")?"Active companion":"Unlocked companion");
        prose(detail,f.optString("Description"));
        line(detail,"Level",value(f,"Level"));
        error(detail,f);
        if(locked)prose(detail,"This faery has not been unlocked in the current character's campaign.");
        else if(f.optBoolean("Selected"))prose(detail,"This companion is currently selected for the character's faery control.");
        attach(c[1],detail);
    }
}
