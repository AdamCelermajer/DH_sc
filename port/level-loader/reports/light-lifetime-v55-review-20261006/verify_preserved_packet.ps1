param([string]$PacketRoot=$PSScriptRoot)
$ErrorActionPreference='Stop'
function Check([bool]$Pass,[string]$Description){if(-not $Pass){throw $Description}}
function ReadJson([string]$Name){Get-Content -LiteralPath (Join-Path $PacketRoot $Name) -Raw | ConvertFrom-Json}
$d=ReadJson 'original-lightpoint-dtor-v55.json'
Check ($d.validation -eq 'PASS' -and $d.actual_original_C1_and_full_Dtor_bodies -and $d.actual_original_IReferenceCounted_drop) 'Original reached LightPoint body proof absent'
Check ($d.original_sha256 -eq '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80') 'ELF provenance mismatch'
Check ($d.rows.Count -eq 2) 'Expected absent/present node cases'
foreach($row in $d.rows){
 Check ($row.actor120_after -eq 0 -and $row.automatic_list_bytes_unchanged -and $row.automatic_list_still_borrows_old_actor) 'Actor reset/borrow preservation mismatch'
 $ev=@($row.events)
 Check ($ev[0].receiver_offset -eq '0x180') 'attachedTo release must precede node release'
 if($row.node_present){Check ($row.node_reference_count_after -eq 1 -and $ev[1].op -eq 'actual node IReferenceCounted.drop' -and $ev[1].refcount_before -eq 2 -and $ev[2].receiver_offset -eq '0x168' -and $ev[3].op -eq 'actual ObjectBase destructor body') 'LightPoint source release order mismatch'}
 else{Check ($row.node_reference_count_after -eq 2 -and $ev[1].receiver_offset -eq '0x168' -and $ev[2].op -eq 'actual ObjectBase destructor body') 'Absent node order mismatch'}
}
$r=ReadJson 'original-lightset-reset-v55.json'
Check ($r.validation -eq 'PASS' -and $r.actual_original_ResetLightSets_body -and $r.rows.Count -eq 4) 'Original ResetLightSets proof absent'
foreach($row in $r.rows){Check ($row.all25_slots_cleared -and $row.shared_light_refcount_after -eq 1 -and $row.field1a0_after -eq 0 -and $row.names_bytes_preserved -and $row.automatic_vector_preserved) 'ResetLightSets invariant mismatch'}
$a=ReadJson 'original-lightset-assignment-v55.json'
Check ($a.validation -eq 'PASS' -and $a.actual_original_assignment_and_vector_assignment_bodies -and $a.rows.Count -eq 3) 'Original assignment proof absent'
foreach($row in $a.rows){
 Check ($row.auto_storage_reused -and $row.same_destination_name_fields) 'Assignment storage/Name fields changed'
 if($row.self_assignment){Check ($row.auto_count_after -eq 2 -and $row.old_light_ref_after -eq 2 -and $row.field1a0_after -eq 77) 'Self-assignment preservation mismatch'}
 else{Check ($row.auto_count_after -eq 0 -and $row.old_light_ref_after -eq 1 -and $row.field1a0_after -eq 0 -and (($row.destination_names -join '|') -eq 'PlayerLight|SceneLight|CameraLight|MonsterLight')) 'Fresh-layout assignment mismatch'; $expected=if($row.source_light_present){3}else{2}; Check ($row.new_light_ref_after -eq $expected) 'Source light ref retention mismatch'}
}
$j=ReadJson 'authored51-lightset-mode-matrix-v55.json'
Check ($j.source_definitions -eq 51 -and $j.rows.Count -eq 51 -and $j.actual_original_TinyXML_body_executed_for_each_definition -and -not $j.whole_runtime_or_procedural_seed_verified) 'All51 claim bounds mismatch'
Check ($j.authored_mode_presence_counts.'(True, True)' -eq 45 -and $j.authored_mode_presence_counts.'(True, False)' -eq 1 -and $j.authored_mode_presence_counts.'(False, False)' -eq 5) 'Authored mode counts mismatch'
Check (@($j.missing_source_lightset_edges).Count -eq 0 -and @($j.unmapped_filename_looking_attributes).Count -eq 2 -and @($j.parser_error_definitions).Count -eq 6) 'Resource/metadata/parser counts mismatch'
Check (@($j.rows | Where-Object {$_.level_identity_source_fields.form -eq 'procedural'}).Count -eq 35) 'Procedural route count mismatch'
Check (@($j.rows | Where-Object {$_.direct_original_definition_LevelConfig -eq 1}).Count -eq 15) 'Direct config count mismatch'
Check (@($j.rows | Where-Object {$_.level_identity_source_fields.form -eq 'fixed' -and $_.direct_original_definition_LevelConfig -eq 0}).Count -eq 1) 'Fixed debug route count mismatch'
Check (@($j.rows | Where-Object {$_.level -eq 'SWAMP'}).Count -eq 1 -and @($j.rows | Where-Object {$_.level -eq 'SWAMP_02'}).Count -eq 1) 'SWAMP identities collapsed'
$single=@($j.rows | Where-Object {$_.authored_mode_presence.light_set -and -not $_.authored_mode_presence.fixed_light_set})
Check ($single.Count -eq 1 -and $single[0].level -eq 'RANDOMFORALEX') 'Single regular-only authored row mismatch'
'PASS: preserved original proof invariants and all51 coverage bounds; no new ARM/runtime execution.'
