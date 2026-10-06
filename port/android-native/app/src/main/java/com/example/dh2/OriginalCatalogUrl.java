package com.example.dh2;

import android.util.Base64;
import java.nio.charset.StandardCharsets;
import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

final class OriginalCatalogUrl {
    static final String[] LANGUAGES={"EN","FR","DE","IT","SP","JP","KR","CN","BR"};
    static final String[] LOADING={"Loading...","Chargement...","Lädt...","Caricamento in corso...","Cargando...","読み込んでいます...","불러오는 중...","载入中……","Carregando..."};
    static int language(int value){return value>=0&&value<LANGUAGES.length?value:0;}
    // Original GLUtils.Encrypter.getValue/PadString/crypt from the supplied DEX.
    static String encryptDeviceId(String deviceId) throws java.security.GeneralSecurityException {
        byte[] delta={-48,-37,4,31,-39,-27,52,-36,41,13,-32,32,-35,20,5,11,-8,-33,-5,-9,5,-6,-4,0,0};
        byte[] encoded=new byte[24];encoded[0]=118;
        for(int i=1;i<encoded.length;i++)encoded[i]=(byte)(encoded[i-1]+delta[i]);
        String text=deviceId==null?"GLOFT_EMU_001":deviceId;
        int padding=16-text.length()%16;
        if(padding<16){StringBuilder padded=new StringBuilder(text);while(padding-->0)padded.append(' ');text=padded.toString();}
        Cipher cipher=Cipher.getInstance("AES/ECB/PKCS5Padding");
        // Modern providers require the key algorithm AES; original used ECB.
        cipher.init(Cipher.ENCRYPT_MODE,new SecretKeySpec(Base64.decode(encoded,Base64.DEFAULT),"AES"));
        return Base64.encodeToString(cipher.doFinal(text.getBytes(StandardCharsets.UTF_8)),Base64.NO_WRAP);
    }
    static String build(int language,String deviceId,String country,String manufacturer,String model,String firmware,int height) throws java.security.GeneralSecurityException {
        return ("http://ingameads.gameloft.com/redir/android/index.php?from=D2SS&lg="+LANGUAGES[language(language)]
            +"&udid="+encryptDeviceId(deviceId)+"&d="+manufacturer+"_"+model+"&f="+firmware
            +"&ver=1.0.2&country="+country+"&height="+height).replace(" ","")+"&enc=1";
    }
}
