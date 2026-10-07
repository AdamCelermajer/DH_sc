import java.security.MessageDigest
import org.gradle.api.DefaultTask
import org.gradle.api.file.DirectoryProperty
import org.gradle.api.file.RegularFileProperty
import org.gradle.api.tasks.InputFile
import org.gradle.api.tasks.OutputDirectory
import org.gradle.api.tasks.TaskAction

plugins {
    alias(libs.plugins.android.application)
}

val dh2SourceRoot = rootProject.file("reconstruction-source").takeIf { it.exists() }
    ?: rootProject.file("../..")

// One APK owns the original cache. Override this explicit input with
// -Pdh2OriginalCache=/path/to/the/supplied.zip on another development machine.
val originalCache = providers.gradleProperty("dh2OriginalCache")
    .orElse("C:/Users/adamc/Downloads/Dungeon-Hunter-2-HD-v1-0-2-cache.zip")
val generatedCacheAssets = layout.buildDirectory.dir("generated/dh2-original-cache/assets")
abstract class BundleOriginalCache : DefaultTask() {
    @get:InputFile abstract val cacheZip: RegularFileProperty
    @get:OutputDirectory abstract val assets: DirectoryProperty
    @TaskAction fun bundle() {
        val source = cacheZip.get().asFile
        require(source.isFile) { "Supply the original cache ZIP with -Pdh2OriginalCache=..." }
        val digest = MessageDigest.getInstance("SHA-256")
        source.inputStream().use { input ->
            val buffer = ByteArray(1024 * 1024)
            while (true) {
                val count = input.read(buffer)
                if (count < 0) break
                digest.update(buffer, 0, count)
            }
        }
        val hash = digest.digest().joinToString("") { "%02x".format(it.toInt() and 255) }
        require(hash == "3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679") {
            "Original cache bytes differ from the supplied canonical archive"
        }
        val destination = assets.get().asFile
        destination.mkdirs()
        source.copyTo(destination.resolve("dh2-original-cache.zip"), overwrite = true)
    }
}
val bundleOriginalCache = tasks.register<BundleOriginalCache>("bundleOriginalCache") {
    cacheZip.set(file(originalCache.get()))
    assets.set(generatedCacheAssets)
}
tasks.named("preBuild").configure { dependsOn(bundleOriginalCache) }

android {
    namespace = "com.example.dh2"
    ndkVersion = "29.0.14206865"
    compileSdk {
        version = release(37)
    }

    defaultConfig {
        applicationId = "com.example.dh2"
        minSdk = 24
        targetSdk = 37
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        ndk { abiFilters += listOf("arm64-v8a", "x86_64") }
        externalNativeBuild {
            cmake {
                arguments += "-DDH2_SOURCE_DIR=${dh2SourceRoot.invariantSeparatorsPath}"
            }
        }
    }

    buildTypes {
        release {
            optimization {
                enable = true
                packageScope = setOf("androidx.**", "kotlin.**", "kotlinx.**")
            }
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    androidResources {
        // Native positional reads use an APK descriptor, without copying or
        // extracting the 433 MB archive at installation or startup.
        noCompress += "zip"
        noCompress += listOf("wav","mp4")
        // Preserve the original cache's .nomedia entry as catalogued. Android's
        // default .* filter otherwise silently drops this verified asset.
        ignoreAssetsPattern = "!.svn:!.git:!.ds_store:!*.scc:!CVS:!thumbs.db:!picasa.ini:!*~"
    }
    sourceSets.getByName("main").assets.directories.add(generatedCacheAssets.get().asFile.absolutePath)
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
}

dependencies {
    implementation(libs.androidx.appcompat)
    implementation(libs.androidx.core.ktx)
    implementation(libs.material)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.espresso.core)
    androidTestImplementation(libs.androidx.junit)
}
