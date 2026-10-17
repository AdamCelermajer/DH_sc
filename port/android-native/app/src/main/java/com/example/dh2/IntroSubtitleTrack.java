package com.example.dh2;

import java.util.Locale;

/** Timed captions transcribed from the original MyVideoView subtitle table. */
final class IntroSubtitleTrack {
    private static final String[][] LINES = {
        {
            "Gothicus, land of fear, land... of destiny.",
            "Now comes a tale born of sorrow and blood.",
            "The tale of a kingdom condemned by the gods,",
            "doomed to dark times in the age of two sons.",
            "Two sons cursed with the strength of immortals;",
            "two sons, cursed by the dread fate of kings.",
            "Armies are raised. Dark powers awake.",
            "And one prince comes to power on a tide of new evil.",
            "But the true tale of sorrow has only begun.",
            "Let this new chapter of Gothicus",
            "be written in fire..."
        },
        {
            "Gothicus, terre de peur, terre... du destin.",
            "Voici une histoire née de la tristesse et du sang.",
            "L'histoire d'un royaume condamné par les dieux,",
            "condamné à l'obscurité quand viendraient deux fils.",
            "Deux fils maudits par le pouvoir de l'immortalité.",
            "Deux fils maudits par le terrible destin des rois.",
            "Les armées se dressent. Les puissances occultes se réveillent.",
            "Et un prince accède au pouvoir alors qu'un nouveau mal se répand.",
            "Mais cette triste histoire n'en est qu'à son commencement.",
            "Le nouveau chapitre de Gothicus...",
            "s'écrira en lettres de feu..."
        },
        {
            "Gothicus, Land der Furcht, Land... des Schicksals.",
            "Diese Legende entstand aus Trauer und Blut.",
            "Die Geschichte eines Reiches, von den Göttern verdammt,",
            "im Zeitalter zweier Söhne in Dunkelheit verfallen.",
            "Zwei Söhne, auf denen der Fluch der Unsterblichkeit lastet.",
            "Zwei Söhne, mit dem schrecklichen Schicksal der Könige geschlagen.",
            "Heere sammeln sich. Dunkle Kräfte erwachen.",
            "Ein Prinz erringt durch eine Woge neuen Übels die Macht.",
            "Doch die wahre Tragödie hat erst begonnen.",
            "Dieses neue Kapitel des Reiches Gothicus",
            "wird mit Feuer geschrieben..."
        },
        {
            "Gothicus, terra di orrori, terra... di destino.",
            "Questa è una storia nata nel sangue e nel dolore;",
            "La storia di un regno condannato dagli dei",
            "all'oscura epoca dei due figli.",
            "Due figli maledetti dal potere degli immortali.",
            "Due figli maledetti dal terribile destino dei Re.",
            "Gli eserciti si radunano, oscuri poteri si risvegliano.",
            "Alla fine, un principe sale al potere cavalcando l'onda del male.",
            "Ma la vera storia di sangue e dolore è appena iniziata.",
            "Che questo capitolo della saga di Gothicus",
            "Sia scritto nel fuoco..."
        },
        {
            "ゴシカス – それは運命に翻弄され、暗い歴史を持つ王国…。",
            "かつての英雄である国王と、血塗られた",
            "運命の下に生まれてきた2人の息子の物語。",
            "父親と同様に不死の力を持つ2人は、",
            "王座をめぐる確執から争いを始める。",
            "やがて戦いの血と炎の匂いは永らく眠っていた",
            "闇の力を呼び起こしてしまう。",
            "戦いは終わり、勝利した王子は国王の座を手にする。",
            "これがゴシカスを襲う悲劇の始まりである。",
            "ゴシカスの運命をかけて、",
            "新たなる歴史が幕を開ける…。"
        },
        {
            "고디커스, 공포의 땅, 운명의... 땅.",
            "슬픔과 피에서 비롯된 이야기가 시작됐다.",
            "신들에게 비난받고 어둠의 시대를 보내는 왕국의 이야기가 시작됐다.",
            "신들에게 비난받고 어둠의 시대를 보내는 왕국의 이야기가 시작됐다.",
            "두 왕자는 불멸의 저주를 받고,",
            "잔혹한 왕의 운명을 타고났다.",
            "군대가 결성되고 어둠의 힘이 깨어났다.",
            "한 명의 왕자는 새로운 악의 힘으로 왕권을 장악했다.",
            "그러나 진정 안타까운 이야기는 이제 막 시작되었을 뿐이다.",
            "고디커스의 새로운 장은",
            "불로 쓰일 것이다..."
        },
        {
            "哥西卡斯，恐惧之城，命运之邦……",
            "此传说源于悲伤和鲜血，",
            "此王国遭受众神的诅咒，",
            "一个王国，两个王子，黑暗时代必定会来临。",
            "两个王子，带着邪恶的不死力量，被诸王的厄运所诅咒。",
            "两个王子，带着邪恶的不死力量，被诸王的厄运所诅咒。",
            "同室操戈，战火在即；邪恶力量，卷土重来。",
            "一位王子登上了王座，却再一次将邪恶力量引到世间。",
            "至此，真正的悲伤传说拉开序幕。",
            "哥西卡斯的新篇章，",
            "将在血与火中开启……"
        },
        {
            "Gothicus, tierra del miedo, tierra del destino...",
            "Esta es una historia de angustia y sangre;",
            "La historia de un reino condenado por los dioses,",
            "a tiempos oscuros en la era de los dos hijos...",
            "Dos hijos malditos con una fuerza inmortal.",
            "Dos hijos, maldecidos con el horrible destino de los reyes.",
            "Se crean ejércitos. Los poderes oscuros despiertan.",
            "Uno de los príncipes toma el poder, inmerso en una nueva ola de maldad.",
            "Pero esta historia de pesar y lamentos no ha hecho más que empezar.",
            "Este nuevo capítulo de Gothicus...",
            "se escribirá con letras de fuego..."
        }
    };

    private IntroSubtitleTrack() { }

    static int languageIndex(Locale locale) {
        if (locale == null) return 0;
        switch (locale.getLanguage()) {
            case "fr": return 1;
            case "de": return 2;
            case "it": return 3;
            case "ja": return 4;
            case "ko": return 5;
            case "zh": return "CN".equalsIgnoreCase(locale.getCountry()) ? 6 : 0;
            case "es": return 7;
            default: return 0;
        }
    }

    static String textAt(int positionMs, int languageIndex) {
        if (languageIndex < 0 || languageIndex >= LINES.length) languageIndex = 0;
        int elapsed = positionMs - 7300;
        int line = -1;
        if (elapsed > 3000 && elapsed < 7000) line = 0;
        else if (elapsed > 7000 && elapsed < 10000) line = 1;
        else if (elapsed > 10000 && elapsed < 13000) line = 2;
        else if (elapsed > 13000 && elapsed < 17000) line = 3;
        else if (elapsed > 18000 && elapsed < 21000) line = 4;
        else if (elapsed > 21000 && elapsed < 25000) line = 5;
        else if (elapsed > 25000 && elapsed < 28000) line = 6;
        else if (elapsed > 28000 && elapsed < 32000) line = 7;
        else if (elapsed > 32000 && elapsed < 36000) line = 8;
        else if (elapsed > 36000 && elapsed < 39000) line = 9;
        else if (elapsed > 39000 && elapsed < 41000) line = 10;
        return line < 0 ? "" : LINES[languageIndex][line];
    }
}
