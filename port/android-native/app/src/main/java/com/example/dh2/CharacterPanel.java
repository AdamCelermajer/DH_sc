package com.example.dh2;

import android.content.Context;
import android.graphics.Color;
import android.graphics.Bitmap;
import android.text.Html;
import android.view.Gravity;
import android.view.View;
import android.widget.*;
import org.json.JSONArray;
import org.json.JSONObject;

/** Modal native view of the live source-backed character authority. */
final class CharacterPanel extends FrameLayout {
    interface Host {
        void refresh(CharacterPanel panel);
        void action(CharacterPanel panel,int operation,int index,int slot);
        void close();
    }
    private final Host host;
    private final LinearLayout content,tabs;
    private final TextView title,message;
    private JSONObject snapshot;
    private int tab;
    private boolean busy;
    private final java.util.Map<String,Bitmap> icons=new java.util.HashMap<>();
    private static final int GOLD=Color.rgb(218,189,123),TEXT=Color.rgb(227,224,212);

    CharacterPanel(Context context,Host host){
        super(context);this.host=host;setClickable(true);setBackgroundColor(Color.rgb(18,19,22));setElevation(dp(24));
        setContentDescription("Character stats inventory skills and faeries");
        LinearLayout panel=new LinearLayout(context);panel.setOrientation(LinearLayout.VERTICAL);panel.setPadding(dp(18),dp(8),dp(18),dp(8));
        addView(panel,new FrameLayout.LayoutParams(-1,-1));
        LinearLayout header=new LinearLayout(context);header.setGravity(Gravity.CENTER_VERTICAL);
        title=text("Character",22,GOLD);header.addView(title,new LinearLayout.LayoutParams(0,dp(44),1));
        Button close=button("Return to game");close.setContentDescription("Close character panel");close.setOnClickListener(v->host.close());header.addView(close);panel.addView(header);
        tabs=new LinearLayout(context);String[] names={"Stats","Equipment","Skills","Faeries"};
        for(int i=0;i<names.length;i++){final int selected=i;Button b=button(names[i]);b.setContentDescription("Character "+names[i]+" tab");b.setOnClickListener(v->{tab=selected;render();});tabs.addView(b,new LinearLayout.LayoutParams(0,dp(44),1));}panel.addView(tabs);
        message=text("Loading character…",13,TEXT);message.setPadding(dp(6),dp(3),dp(6),dp(3));panel.addView(message);
        ScrollView scroll=new ScrollView(context);scroll.setFillViewport(true);content=new LinearLayout(context);content.setOrientation(LinearLayout.VERTICAL);scroll.addView(content);panel.addView(scroll,new LinearLayout.LayoutParams(-1,0,1));
    }
    private int dp(float value){return Math.round(value*getResources().getDisplayMetrics().density);}
    private TextView text(String value,int size,int color){TextView view=new TextView(getContext());view.setText(value);view.setTextSize(size);view.setTextColor(color);view.setGravity(Gravity.CENTER_VERTICAL);return view;}
    private Button button(String value){Button b=new Button(getContext());b.setText(value);b.setTextSize(13);b.setTextColor(TEXT);b.setAllCaps(false);return b;}
    private void line(String label,String value){LinearLayout row=new LinearLayout(getContext());row.setPadding(dp(8),dp(2),dp(8),dp(2));row.addView(text(label,14,TEXT),new LinearLayout.LayoutParams(0,dp(30),1));TextView number=text(value,14,GOLD);number.setGravity(Gravity.RIGHT|Gravity.CENTER_VERTICAL);row.addView(number,new LinearLayout.LayoutParams(0,dp(30),1));content.addView(row);}
    private void heading(String name){TextView view=text(name,17,GOLD);view.setPadding(dp(8),dp(10),dp(8),dp(4));content.addView(view);}
    private static String value(JSONObject row,String key){Object value=row.opt(key);return value==null||value==JSONObject.NULL?"—":String.valueOf(value);}
    private TextView description(String html){TextView view=text("",13,TEXT);view.setText(Html.fromHtml(html.replace("\n","<br>"),Html.FROM_HTML_MODE_LEGACY));view.setPadding(dp(8),0,dp(8),dp(6));return view;}
    private void command(int op,int index,int slot){if(busy)return;busy=true;message.setText("Applying…");render();host.action(this,op,index,slot);}
    void update(String json){
        busy=false;
        try{JSONObject next=new JSONObject(json);if(!next.optBoolean("ready")){snapshot=null;message.setText(next.optString("error","Character unavailable"));render();return;}
            snapshot=next;message.setText("Gold "+next.optInt("gold")+"   •   Skill points "+next.optInt("skillPoints")+"   •   Equipment set "+(next.optInt("equipmentSet")+1));render();
        }catch(Exception error){snapshot=null;message.setText("Character data failed: "+error.getMessage());render();}
    }
    void result(String result){message.setText(result);}
    void icons(java.util.Map<String,Bitmap> values){icons.putAll(values);render();}
    private void iconHeading(LinearLayout parent,String label,String icon,int size,int color){
        LinearLayout row=new LinearLayout(getContext());row.setGravity(Gravity.CENTER_VERTICAL);
        Bitmap bitmap=icons.get(icon);if(bitmap!=null){ImageView view=new ImageView(getContext());view.setImageBitmap(bitmap);view.setScaleType(ImageView.ScaleType.FIT_CENTER);row.addView(view,new LinearLayout.LayoutParams(dp(44),dp(44)));}
        TextView name=text(label,size,color);name.setPadding(dp(8),dp(5),dp(8),dp(5));row.addView(name,new LinearLayout.LayoutParams(0,-2,1));parent.addView(row);
    }
    private void render(){
        content.removeAllViews();for(int i=0;i<tabs.getChildCount();i++)tabs.getChildAt(i).setAlpha(i==tab?1f:.6f);
        if(snapshot==null)return;
        JSONObject stats=snapshot.optJSONObject("stats");if(stats==null)return;
        title.setText(stats.optString("Name").isEmpty()?stats.optString("Class","Character"):stats.optString("Name")+" • "+stats.optString("Class"));
        if(tab==0)stats(stats);else if(tab==1)items(snapshot.optJSONArray("items"));else if(tab==2)skills(snapshot.optJSONArray("skills"));else faeries(snapshot.optJSONArray("faeries"));
    }
    private void stats(JSONObject stats){
        heading("Character");line("Level",value(stats,"Level"));line("Health",value(stats,"HP")+" / "+value(stats,"Max_HP"));line("Mana",value(stats,"MP")+" / "+value(stats,"Max_MP"));line("Experience",value(stats,"XP")+" / "+value(stats,"Max_XP"));
        heading("Attributes • "+stats.optInt("Stat_Points")+" points available");String[] keys={"Stat_Strength","Stat_Dexterity","Stat_Endurance","Stat_Energy"},labels={"Strength","Dexterity","Endurance","Energy"};
        for(int i=0;i<4;i++){final int stat=i;LinearLayout row=new LinearLayout(getContext());row.setGravity(Gravity.CENTER_VERTICAL);row.addView(text(labels[i]+"   "+value(stats,keys[i]),15,TEXT),new LinearLayout.LayoutParams(0,dp(42),1));Button add=button("+ 1");add.setContentDescription("Assign "+labels[i]+" point");add.setEnabled(!busy&&stats.optInt("Stat_Points")>0);add.setOnClickListener(v->command(2,stat,-1));row.addView(add);content.addView(row);}
        String[][] sections={{"Offense","Rating_Attack","Attack rating","Rating_Critical","Critical rating","Damage_Min_Main_Hand","Main hand minimum","Damage_Max_Main_Hand","Main hand maximum","Damage_Min_Off_Hand","Off hand minimum","Damage_Max_Off_Hand","Off hand maximum"},{"Defense","Rating_Defense","Defense rating","Rating_Dodge","Dodge rating","Rating_Block","Block rating","Physical_Armor","Armor"},{"Resistance","Resistance_Fire","Fire","Resistance_Water","Water","Resistance_Lightning","Lightning","Resistance_Earth","Earth","Resistance_Air","Air"},{"Recovery","Regen_HP","Health regeneration","Regen_MP","Mana regeneration","Leech_HP","Health leech","Leech_MP","Mana leech"}};
        for(String[] section:sections){heading(section[0]);for(int j=1;j<section.length;j+=2)line(section[j+1],value(stats,section[j]));}
    }
    private void items(JSONArray items){
        heading("Inventory and equipped items");if(items==null||items.length()==0){line("Inventory","Empty");return;}
        for(int i=0;i<items.length();i++){JSONObject item=items.optJSONObject(i);if(item==null)continue;int index=item.optInt("Index"),slot=item.optInt("EquippedSlot",-1);
            LinearLayout card=new LinearLayout(getContext());card.setOrientation(LinearLayout.VERTICAL);card.setPadding(dp(10),dp(6),dp(10),dp(6));card.setBackgroundColor(i%2==0?Color.argb(95,71,66,52):Color.argb(70,49,48,44));
            String name=item.optString("ItemName","Item");card.setContentDescription("Inventory item "+index+" "+name);iconHeading(card,name+" × "+item.optInt("Quantity",1)+(slot>=0?"   •   "+item.optString("EquipmentSlotName","Equipped slot "+(slot+1)):""),item.optString("ItemIcon"),16,slot>=0?GOLD:TEXT);
            card.addView(description(item.optString("ItemStatsDesc")));for(int p=0;p<item.optInt("ItemMagics");p++)card.addView(description(item.optString("ItemPowers"+p+"Desc")));
            String requirement=item.optString("ItemReqsDesc");if(!requirement.isEmpty())card.addView(description(requirement));
            if(item.has("Error"))card.addView(text(item.optString("Error"),12,Color.rgb(241,150,134)));
            LinearLayout commands=new LinearLayout(getContext());commands.setGravity(Gravity.RIGHT);TextView price=text("Value "+item.optString("ItemValueString","—"),13,TEXT);commands.addView(price,new LinearLayout.LayoutParams(0,dp(40),1));
            Button equip=button(slot>=0?"Unequip":"Equip");equip.setContentDescription((slot>=0?"Unequip ":"Equip ")+name);equip.setEnabled(!busy&&(slot>=0||item.optBoolean("ItemEquippable")));equip.setOnClickListener(v->command(slot>=0?1:0,index,slot));commands.addView(equip);card.addView(commands);content.addView(card);
        }
    }
    private void skills(JSONArray skills){
        heading("Skill tree • "+snapshot.optInt("skillPoints")+" points available");if(skills==null)return;
        for(int i=0;i<skills.length();i++){JSONObject skill=skills.optJSONObject(i);if(skill==null)continue;int index=skill.optInt("Index"),level=skill.optInt("SkillLevel"),slot=skill.optInt("EquippedSlot",-1),required=skill.optInt("RequiredLevel");
            iconHeading(content,skill.optString("SkillName","Skill "+(index+1))+"   •   Level "+level+(slot>=0?"   •   Slot "+(slot+1):""),skill.optString("SkillIcon"),17,GOLD);content.addView(description(skill.optString("SkillDescription")));content.addView(description(skill.optString("SkillCurrLevel")));if(!skill.optString("SkillNextLevel").isEmpty())content.addView(description(skill.optString("SkillNextLevel")));
            if(skill.has("Error"))content.addView(text(skill.optString("Error"),12,Color.rgb(241,150,134)));
            LinearLayout commands=new LinearLayout(getContext());commands.setGravity(Gravity.CENTER_VERTICAL);TextView unlock=text("Requires level "+required,12,TEXT);commands.addView(unlock,new LinearLayout.LayoutParams(0,dp(42),1));Button learn=button(level==0?"Learn":"Improve");learn.setEnabled(!busy&&snapshot.optInt("skillPoints")>0&&skill.optBoolean("SkillUnlocked")&&!skill.has("Error"));learn.setContentDescription("Train skill "+index);learn.setOnClickListener(v->command(3,index,-1));commands.addView(learn);
            if(level>0&&skill.optBoolean("SkillAssignable")){for(int s=0;s<3;s++){final int selected=s;Button equip=button("Slot "+(s+1));equip.setEnabled(!busy);equip.setOnClickListener(v->command(4,index,selected));commands.addView(equip);}}
            content.addView(commands);
        }
    }
    private void faeries(JSONArray faeries){
        heading("Faery companions");if(faeries==null)return;
        for(int i=0;i<faeries.length();i++){JSONObject f=faeries.optJSONObject(i);if(f==null)continue;int state=f.optInt("State");heading(f.optString("Name","Faery "+(f.optInt("Index")+1))+(state==0?" • Locked":f.optBoolean("Selected")?" • Selected":""));content.addView(description(f.optString("Description")));line("Level",value(f,"Level"));if(f.has("Error"))content.addView(text(f.optString("Error"),12,Color.rgb(241,150,134)));if(state==0)content.addView(description("Not unlocked in this character's campaign."));}
    }
}
