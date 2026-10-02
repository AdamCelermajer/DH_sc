package com.samsungapps.plasma;

import android.util.Xml;
import java.io.IOException;
import java.io.StringReader;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.HashMap;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;
import org.xmlpull.v1.XmlSerializer;

/* JADX INFO: loaded from: classes.dex */
final class k {

    /* JADX INFO: renamed from: a, reason: collision with root package name */
    private static final String f294a = "";
    private static final String b = "SamsungProtocol";
    private static final String c = "request";
    private static final String d = "response";
    private static final String e = "list";
    private static final String f = "param";
    private static final String g = "value";
    private static final String h = "errorString";

    k() {
    }

    // Reconstructed from k.smali. JADX placed a return in the event loop and
    // invented checked catches around assignments. The DEX advances until
    // END_DOCUMENT and returns its partially parsed response on parser/I/O errors.
    static m a(String str) {
        ArrayList lists = new ArrayList();
        StringReader reader = new StringReader(str);
        m response = null;
        try {
            XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
            factory.setNamespaceAware(false);
            XmlPullParser parser = factory.newPullParser();
            parser.setInput(reader);
            String valueName = f294a;
            String valueText = f294a;
            String currentTag = f294a;
            HashMap currentList = null;
            for (int event = parser.getEventType(); event != XmlPullParser.END_DOCUMENT; event = parser.next()) {
                switch (event) {
                    case XmlPullParser.START_TAG:
                        currentTag = parser.getName();
                        if (currentTag.equals(b)) {
                            response = new m();
                        } else if (currentTag.equals(d)) {
                            if (response != null) {
                                response.b(i.b(parser.getAttributeValue(f294a, "id")));
                                response.a(parser.getAttributeValue(f294a, "name"));
                                response.a(i.b(parser.getAttributeValue(f294a, "transactionId")));
                                response.c(i.b(parser.getAttributeValue(f294a, "totalCount")));
                                response.d(i.b(parser.getAttributeValue(f294a, "startNum")));
                                response.e(i.b(parser.getAttributeValue(f294a, "endNum")));
                                response.f(i.b(parser.getAttributeValue(f294a, "returnCode")));
                            }
                        } else if (currentTag.equals(h)) {
                            if (response != null) {
                                response.g(i.b(parser.getAttributeValue(f294a, "errorCode")));
                            }
                        } else if (currentTag.equals(e)) {
                            currentList = new HashMap();
                        } else if (currentTag.equals(g)) {
                            valueName = parser.getAttributeValue(f294a, "name");
                        }
                        break;
                    case XmlPullParser.TEXT:
                        if (currentTag.equals(g)) {
                            valueText = parser.getText();
                        } else if (currentTag.equals(h) && response != null) {
                            response.b(parser.getText());
                        }
                        break;
                    case XmlPullParser.END_TAG:
                        String endTag = parser.getName();
                        if (endTag.equals(b)) {
                            if (response != null) {
                                response.a(lists);
                            }
                        } else if (endTag.equals(e)) {
                            lists.add(currentList);
                            currentList = null;
                        } else if (endTag.equals(g)) {
                            currentList.put(valueName, valueText);
                            valueName = f294a;
                            valueText = f294a;
                        }
                        currentTag = f294a;
                        break;
                    default:
                        break;
                }
            }
        } catch (IOException e) {
            a.a(e);
        } catch (XmlPullParserException e) {
            a.a(e);
        } finally {
            reader.close();
        }
        return response;
    }

    static String a(l lVar, b bVar) {
        HashMap mapD = lVar.d();
        int size = mapD != null ? mapD.size() : 0;
        XmlSerializer xmlSerializerNewSerializer = Xml.newSerializer();
        StringWriter stringWriter = new StringWriter();
        try {
            xmlSerializerNewSerializer.setOutput(stringWriter);
            xmlSerializerNewSerializer.startDocument("UTF-8", true);
            xmlSerializerNewSerializer.startTag(f294a, b);
            xmlSerializerNewSerializer.attribute(f294a, "deviceModel", bVar.e());
            xmlSerializerNewSerializer.attribute(f294a, "networkType", "1");
            String str = String.format("%d", Integer.valueOf(bVar.b()));
            if (com.samsungapps.plasma.d.b) {
                str = "000";
            }
            xmlSerializerNewSerializer.attribute(f294a, "mcc", str);
            xmlSerializerNewSerializer.attribute(f294a, "mnc", String.format("%02d", Integer.valueOf(bVar.c())));
            xmlSerializerNewSerializer.attribute(f294a, "csc", bVar.d());
            xmlSerializerNewSerializer.attribute(f294a, "lang", "EN");
            xmlSerializerNewSerializer.attribute(f294a, "version", "1.0");
            xmlSerializerNewSerializer.startTag(f294a, c);
            xmlSerializerNewSerializer.attribute(f294a, "id", String.valueOf(lVar.c()));
            xmlSerializerNewSerializer.attribute(f294a, "name", lVar.b());
            xmlSerializerNewSerializer.attribute(f294a, "numParam", String.valueOf(size));
            xmlSerializerNewSerializer.attribute(f294a, "transactionId", String.valueOf(lVar.a()));
            if (mapD != null) {
                for (Object key : mapD.keySet()) {
                    String str2 = (String) key;
                    xmlSerializerNewSerializer.startTag(f294a, f);
                    xmlSerializerNewSerializer.attribute(f294a, "name", str2);
                    try {
                        xmlSerializerNewSerializer.text((String) mapD.get(str2));
                    } catch (Exception e2) {
                    }
                    xmlSerializerNewSerializer.endTag(f294a, f);
                }
            }
            xmlSerializerNewSerializer.endTag(f294a, c);
            xmlSerializerNewSerializer.endTag(f294a, b);
            xmlSerializerNewSerializer.endDocument();
        } catch (IOException e3) {
            a.a(e3);
        } finally {
            try {
                stringWriter.flush();
                stringWriter.close();
            } catch (IOException e4) {
            }
        }
        return stringWriter.toString();
    }
}
