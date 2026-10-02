package local.dh2.uitest;

import android.app.Instrumentation;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.Base64;
import android.util.Xml;
import android.view.accessibility.AccessibilityNodeInfo;
import java.io.StringWriter;
import java.nio.charset.StandardCharsets;
import org.xmlpull.v1.XmlSerializer;

/** Reads the current accessibility tree without waiting for an animated UI to idle. */
public final class Snapshot extends Instrumentation {
    private int count;
    private static String text(CharSequence value) { return value == null ? "" : value.toString(); }

    @Override public void onCreate(Bundle arguments) {
        super.onCreate(arguments);
        start();
    }

    private void write(XmlSerializer xml, AccessibilityNodeInfo node, int depth) throws Exception {
        if (depth > 80 || ++count > 10000) throw new IllegalStateException("UI tree exceeds bound");
        Rect bounds = new Rect();
        node.getBoundsInScreen(bounds);
        xml.startTag(null, "node");
        xml.attribute(null, "text", text(node.getText()));
        xml.attribute(null, "resource-id", text(node.getViewIdResourceName()));
        xml.attribute(null, "class", text(node.getClassName()));
        xml.attribute(null, "package", text(node.getPackageName()));
        xml.attribute(null, "content-desc", text(node.getContentDescription()));
        xml.attribute(null, "bounds", "[" + bounds.left + "," + bounds.top + "][" + bounds.right + "," + bounds.bottom + "]");
        xml.attribute(null, "enabled", Boolean.toString(node.isEnabled()));
        for (int i = 0; i < node.getChildCount(); ++i) {
            AccessibilityNodeInfo child = node.getChild(i);
            if (child != null) write(xml, child, depth + 1);
        }
        xml.endTag(null, "node");
    }

    @Override public void onStart() {
        Bundle result = new Bundle();
        int code = 0;
        try {
            AccessibilityNodeInfo root = null;
            for (int i = 0; i < 50 && root == null; ++i) {
                root = getUiAutomation().getRootInActiveWindow();
                if (root == null) SystemClock.sleep(100);
            }
            if (root == null) throw new IllegalStateException("No active UI root yet");
            StringWriter output = new StringWriter();
            XmlSerializer xml = Xml.newSerializer();
            xml.setOutput(output);
            xml.startTag(null, "hierarchy");
            write(xml, root, 0);
            xml.endTag(null, "hierarchy");
            xml.flush();
            result.putString("hierarchy", Base64.encodeToString(output.toString().getBytes(StandardCharsets.UTF_8), Base64.NO_WRAP));
        } catch (Exception error) {
            code = 1;
            result.putString("error", error.toString());
        }
        finish(code, result);
    }
}
