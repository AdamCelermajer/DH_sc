package com.kddi.market.alml.lib;

import android.accounts.AccountManager;
import android.accounts.AuthenticatorDescription;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

/* JADX INFO: loaded from: classes.dex */
class ApiUtil {

    /* JADX INFO: renamed from: a, reason: collision with root package name */
    private static final String f187a = "com.kddi.android.auoneidsetting";
    private static /* synthetic */ int[] b;

    enum TokenApiType {
        GET_AUONE_TOKEN,
        GET_AU_TOKEN,
        GET_AUONE_OTHER,
        GET_AU_OTHER,
        GET_OPEN_ID,
        GET_EZNO;

        /* JADX DEBUG: Replace access to removed values field (g) with 'values()' method */
        /* JADX INFO: renamed from: values, reason: to resolve conflict with enum method */
        public static TokenApiType[] valuesCustom() {
            TokenApiType[] tokenApiTypeArrValuesCustom = values();
            int length = tokenApiTypeArrValuesCustom.length;
            TokenApiType[] tokenApiTypeArr = new TokenApiType[length];
            System.arraycopy(tokenApiTypeArrValuesCustom, 0, tokenApiTypeArr, 0, length);
            return tokenApiTypeArr;
        }
    }

    static /* synthetic */ int[] $SWITCH_TABLE$com$kddi$market$alml$lib$ApiUtil$TokenApiType() {
        int[] iArr = b;
        if (iArr == null) {
            iArr = new int[TokenApiType.valuesCustom().length];
            try {
                iArr[TokenApiType.GET_AUONE_OTHER.ordinal()] = 3;
            } catch (NoSuchFieldError e) {
            }
            try {
                iArr[TokenApiType.GET_AUONE_TOKEN.ordinal()] = 1;
            } catch (NoSuchFieldError e2) {
            }
            try {
                iArr[TokenApiType.GET_AU_OTHER.ordinal()] = 4;
            } catch (NoSuchFieldError e3) {
            }
            try {
                iArr[TokenApiType.GET_AU_TOKEN.ordinal()] = 2;
            } catch (NoSuchFieldError e4) {
            }
            try {
                iArr[TokenApiType.GET_EZNO.ordinal()] = 6;
            } catch (NoSuchFieldError e5) {
            }
            try {
                iArr[TokenApiType.GET_OPEN_ID.ordinal()] = 5;
            } catch (NoSuchFieldError e6) {
            }
            b = iArr;
        }
        return iArr;
    }

    private ApiUtil() {
    }

    static Intent createAstSettingIntent() {
        return createSettingIntent(f187a);
    }

    static Bundle createLoginOption(String str, String str2, boolean z) {
        Bundle bundle = new Bundle();
        bundle.putString(com.kddi.market.a.a.h, str);
        bundle.putString(com.kddi.market.a.a.i, str2);
        bundle.putBoolean(com.kddi.market.a.a.j, z);
        return bundle;
    }

    static Intent createSettingIntent(String str) {
        Intent intent = new Intent();
        intent.setAction("android.settings.APPLICATION_DETAILS_SETTINGS");
        intent.setData(Uri.parse("package:" + str));
        intent.setFlags(268435456);
        return intent;
    }

    static boolean existAuOneIdSetting(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(f187a, 1);
            return (packageInfo == null || packageInfo.applicationInfo == null) ? false : true;
        } catch (PackageManager.NameNotFoundException e) {
            return false;
        }
    }

    static boolean existsAuthenticator(Context context, String str) {
        AuthenticatorDescription[] authenticatorTypes = AccountManager.get(context).getAuthenticatorTypes();
        if (authenticatorTypes == null) {
            return false;
        }
        for (AuthenticatorDescription authenticatorDescription : authenticatorTypes) {
            if (str.equals(authenticatorDescription.type)) {
                return true;
            }
        }
        return false;
    }

    static int getAlmlErrorCode(TokenApiType tokenApiType, int i) {
        switch (i) {
            case 0:
                return 0;
            case 1:
            case 5:
            case 6:
            case 8:
                return -99;
            case 3:
                return -1;
            case 4:
            case 13:
            case 3005:
            case 3006:
            case 3007:
            case 3012:
            case 3013:
            case 3014:
            case 3016:
            case 3097:
                return -4;
            case 7:
                return -8;
            case 11:
                return -2;
            case 12:
                return -6;
            case com.kddi.market.a.a.y /* 15 */:
                return -99;
            case 17:
                return getCannotGetError(tokenApiType);
            case 18:
                return -9;
            case 2101:
            case 2102:
            case 2105:
            case 2106:
            case 2107:
            case 2108:
            case 2109:
            case 2110:
            case 2201:
            case 2202:
            case 2203:
            case 2204:
            case 2205:
            case 2206:
            case 2207:
            case 2208:
            case 2209:
            case 2210:
            case 2301:
            case 2302:
            case 2303:
            case 2304:
            case 2400:
            case 2402:
            case 2412:
            case 2413:
            case 2414:
            case 2415:
            case 2416:
            case 2509:
            case 3001:
            case 3002:
            case 3008:
            case 3015:
            case 3098:
            case 3099:
                return -99;
            case 3003:
                return -57;
            case 3094:
                return -3;
            default:
                return -99;
        }
    }

    static int getCannotGetError(TokenApiType tokenApiType) {
        switch ($SWITCH_TABLE$com$kddi$market$alml$lib$ApiUtil$TokenApiType()[tokenApiType.ordinal()]) {
            case 1:
            case 2:
                return -41;
            case 3:
            case 4:
                return -49;
            case 5:
                return -43;
            case 6:
                return -46;
            default:
                return -99;
        }
    }

    /* JADX DEBUG: Failed to insert an additional move for type inference into block B:50:0x0090 */
    /* JADX DEBUG: Failed to insert an additional move for type inference into block B:52:0x0092 */
    /* JADX DEBUG: Failed to insert an additional move for type inference into block B:54:0x0094 */
    /* JADX DEBUG: Failed to insert an additional move for type inference into block B:56:0x0096 */
    /* JADX DEBUG: Failed to insert an additional move for type inference into block B:59:0x0035 */
    /* JADX WARN: Multi-variable type inference failed */
    /* JADX WARN: Type inference failed for: r2v14 */
    /* JADX WARN: Type inference failed for: r2v16 */
    /* JADX WARN: Type inference failed for: r2v17 */
    /* JADX WARN: Type inference failed for: r2v18 */
    /* JADX WARN: Type inference failed for: r2v19 */
    /* JADX WARN: Type inference failed for: r2v20 */
    /* JADX WARN: Type inference failed for: r2v21 */
    /* JADX WARN: Type inference failed for: r2v22 */
    /* JADX WARN: Type inference failed for: r2v23 */
    /* JADX WARN: Type inference failed for: r2v24 */
    /* JADX WARN: Type inference failed for: r2v25 */
    /* JADX WARN: Type inference failed for: r2v4, types: [java.lang.String] */
    /* JADX WARN: Type inference failed for: r2v5 */
    /* JADX WARN: Type inference failed for: r2v6, types: [java.io.BufferedReader] */
    /* JADX WARN: Type inference failed for: r2v7 */
    // Reconstructed from ApiUtil.smali: separate the reused DEX register's
    // file name and reader roles, retaining debug-only error injection and cleanup.
    static Integer getTestErrorCode(Context context) {
        Integer result = null;
        if ((context.getApplicationInfo().flags & 2) == 2) {
            File directory = new File(Environment.getExternalStorageDirectory(), "market");
            if (directory.exists() && directory.isDirectory()) {
                File errorFile = new File(directory, "errorCode.txt");
                if (errorFile.exists() && errorFile.isFile()) {
                    BufferedReader reader = null;
                    try {
                        reader = new BufferedReader(new FileReader(errorFile));
                        result = Integer.decode(reader.readLine());
                    } catch (NumberFormatException e) {
                        e.printStackTrace();
                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    } catch (IOException e) {
                        e.printStackTrace();
                    } finally {
                        if (reader != null) {
                            try {
                                reader.close();
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                        }
                    }
                }
            }
        }
        return result;
    }

    /* JADX WARN: Code duplicated, block: B:12:0x0020 A[ORIG_RETURN, RETURN] */
    /* JADX WARN: Code duplicated, block: B:30:0x0059  */
    // Reconstructed from ApiUtil.smali: retain the four permission bits and
    // NameNotFoundException logging; no account authorization rule is changed.
    static boolean havePermissions(Context context) {
        int permissionBits = 0;
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 4096);
            if (packageInfo != null && packageInfo.requestedPermissions != null) {
                for (String permission : packageInfo.requestedPermissions) {
                    if ("com.kddi.market.permission.USE_ALML".equals(permission)) {
                        permissionBits |= 1;
                    } else if ("android.permission.GET_ACCOUNTS".equals(permission)) {
                        permissionBits |= 2;
                    } else if ("android.permission.MANAGE_ACCOUNTS".equals(permission)) {
                        permissionBits |= 4;
                    } else if ("android.permission.USE_CREDENTIALS".equals(permission)) {
                        permissionBits |= 8;
                    }
                }
            }
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return permissionBits == 15;
    }

    static boolean isAuOneIdSettingEnabled(Context context) {
        if (existAuOneIdSetting(context)) {
            return isPackageEnabled(context, f187a);
        }
        return true;
    }

    static boolean isPackageEnabled(Context context, String str) {
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(str, 1);
            if (packageInfo == null || packageInfo.applicationInfo == null) {
                return false;
            }
            return packageInfo.applicationInfo.enabled;
        } catch (PackageManager.NameNotFoundException e) {
            return false;
        }
    }
}
