/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>

/*
 * Uncomment if you need these.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
*/

#define  LOG_TAG    "libfilter"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

/* Set to 1 to enable debug log traces. */
#define DEBUG 0

static void filter_bw( AndroidBitmapInfo * info, void* pixels)
{
    int row;
    for (row = 0; row < info->height; row++) {
        uint32_t*  line = (uint32_t*)pixels;
        int col;
        double gamma = 2.2;
        for (col = 0; col < info->width; col++) {
            uint32_t color = line[col];
            uint32_t a = (color & 0xff000000);
            uint8_t r = ((color & 0xffff0000) >> 16);
            uint8_t g = ((color & 0xff00ff00) >> 8);
            uint8_t b = ((color & 0xff0000ff));
            uint8_t lum = (.2126 * r) + (.7152 * g) + (.0722 * b);
            uint8_t tint = (255 - lum) * .25;
            line[col] = a | (lum << 16) | (lum << 8) | lum;
        }
        // go to next line
        pixels = (char*)pixels + info->stride;
    }
}

JNIEXPORT void JNICALL Java_com_vandalsoftware_filter_Filter_filterBitmap(JNIEnv * env, jobject  obj, jobject bitmap,  jlong  time_ms)
{
    AndroidBitmapInfo info;
    int ret;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGB_8888 ! error=%d", info.format);
        return;
    }

    void* pixels;

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    filter_bw(&info, pixels);

    AndroidBitmap_unlockPixels(env, bitmap);
}
