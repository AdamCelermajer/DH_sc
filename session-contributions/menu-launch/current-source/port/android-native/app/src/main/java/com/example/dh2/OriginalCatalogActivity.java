package com.example.dh2;

import android.app.Activity;
import android.app.ProgressDialog;
import android.os.Bundle;
import android.content.Intent;
import android.net.Uri;
import android.graphics.Bitmap;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.webkit.WebResourceRequest;
import android.webkit.WebResourceError;
import android.webkit.WebResourceResponse;
import android.util.Log;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.graphics.Color;

public final class OriginalCatalogActivity extends Activity {
    private WebView web;
    private ProgressDialog loading;
    private boolean gameInformation;
    private int language;
    private TextView failure;
    private static final String INFORMATION="http://ingameads.gameloft.com/redir/android/index.php?page=gameinformation";
    private static final String EXTERNAL="http://ingameads.gameloft.com/redir/?from";
    @Override public void onCreate(Bundle state){
        setTheme(android.R.style.Theme_Material_NoActionBar);super.onCreate(state);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        language=OriginalCatalogUrl.language(getIntent().getIntExtra("language",0));
        FrameLayout root=new FrameLayout(this);web=new WebView(this);
        web.setContentDescription("Original More Games catalog");
        web.getSettings().setJavaScriptEnabled(true);web.getSettings().setSupportZoom(false);
        web.getSettings().setDefaultTextEncodingName("utf-8");web.getSettings().setLoadsImagesAutomatically(true);
        web.setVerticalScrollBarEnabled(false);
        root.addView(web,new FrameLayout.LayoutParams(-1,-1));
        failure=new TextView(this);failure.setTextColor(Color.WHITE);failure.setBackgroundColor(0xee25180f);
        failure.setTextSize(18);failure.setPadding(24,24,24,24);failure.setVisibility(android.view.View.GONE);
        root.addView(failure,new FrameLayout.LayoutParams(-1,-2));setContentView(root);
        web.setWebViewClient(new WebViewClient(){
            @Override public void onPageStarted(WebView view,String url,Bitmap icon){
                if(url.startsWith(INFORMATION))gameInformation=true;
                else if(!url.startsWith("http://ingameads.gameloft.com/redir/?from=")&&url.contains("ingameads.gameloft.com"))gameInformation=false;
                if(loading==null){loading=new ProgressDialog(OriginalCatalogActivity.this);loading.setMessage(OriginalCatalogUrl.LOADING[language]);loading.setCancelable(true);loading.setOnCancelListener(dialog->finish());loading.show();}
                Log.i("DH2Front","Original catalog page started");
            }
            @Override public void onPageFinished(WebView view,String url){dismissLoading();Log.i("DH2Front","Original catalog page finished");}
            @Override public boolean shouldOverrideUrlLoading(WebView view,WebResourceRequest request){return navigate(request.getUrl().toString());}
            @Override public boolean shouldOverrideUrlLoading(WebView view,String url){return navigate(url);}
            @Override public void onReceivedError(WebView view,WebResourceRequest request,WebResourceError error){
                if(request.isForMainFrame())failed("The More Games page could not be loaded.","network",error.getErrorCode());
            }
            @Override public void onReceivedHttpError(WebView view,WebResourceRequest request,WebResourceResponse response){
                if(request.isForMainFrame())failed("The More Games service returned an error.","http",response.getStatusCode());
            }
        });
        try{
            // Device.d1 source includes Android ID fallback. Modern Android
            // forbids legacy IMEI/serial reads for this application.
            String id=android.provider.Settings.Secure.getString(getContentResolver(),"android_id");
            int height=getWindowManager().getDefaultDisplay().getHeight();
            String url=OriginalCatalogUrl.build(language,id,java.util.Locale.getDefault().getCountry(),android.os.Build.MANUFACTURER,android.os.Build.MODEL,android.os.Build.VERSION.RELEASE,height);
            web.loadUrl(url);web.requestFocus();
            Log.i("DH2Front","Original More Games catalog opened | language="+language+" | height="+height);
        }catch(java.security.GeneralSecurityException e){failed("The More Games request could not be prepared.","crypto",0);}
    }
    private boolean navigate(String url){
        if(url.startsWith("http://signal-back.com")){finish();return true;}
        if(url.startsWith(EXTERNAL)||url.startsWith("vnd.youtube:")){
            Intent intent=new Intent(Intent.ACTION_VIEW,Uri.parse(url));
            try{startActivity(intent);}catch(android.content.ActivityNotFoundException e){
                if(url.startsWith("vnd.youtube:")){
                    try{startActivity(new Intent(Intent.ACTION_VIEW,Uri.parse("http://www.youtube.com/watch?v="+url.replace("vnd.youtube:",""))));}
                    catch(android.content.ActivityNotFoundException ignored){failed("No browser is available to open this link.","handler",0);}
                }else failed("No browser is available to open this link.","handler",0);
            }
            return true;
        }
        return false;
    }
    private void failed(String message,String kind,int code){
        dismissLoading();failure.setText(message+"\nPress Back to return to the game menu.");failure.setVisibility(android.view.View.VISIBLE);
        Log.w("DH2Front","Original catalog request failed | kind="+kind+" | code="+code);
    }
    private void dismissLoading(){if(loading!=null){loading.dismiss();loading=null;}}
    @Override public void onBackPressed(){if(gameInformation&&web.canGoBack())web.goBack();else finish();}
    @Override protected void onPause(){web.onPause();super.onPause();}
    @Override protected void onResume(){super.onResume();web.onResume();}
    @Override protected void onDestroy(){dismissLoading();if(web!=null){web.stopLoading();web.destroy();}super.onDestroy();}
}
