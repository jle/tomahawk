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

#define MASK_A 0xff000000
#define MASK_R 0xffff0000
#define MASK_G 0xff00ff00
#define MASK_B 0xff0000ff

#define COLOR_A(c) (c & MASK_A)
#define COLOR_R(c) ((c & MASK_R) >> 16)
#define COLOR_G(c) ((c & MASK_G) >> 8)
#define COLOR_B(c) ((c & MASK_B))

typedef uint8_t Filter;

#define NONE  0
#define ROSE  1
#define WEIRD 2
#define CHILL 3

static uint8_t calcY(uint8_t r, uint8_t g, uint8_t b)
{
    return (.2126 * r) + (.7152 * g) + (.0722 * b);
}

static void do_filter(AndroidBitmapInfo* info, void* pixels,
    float intensity, Filter t)
{
    int row;
    for (row = 0; row < info->height; row++) {
        uint32_t*  line = (uint32_t*)pixels;
        int col;
        for (col = 0; col < info->width; col++) {
            uint32_t color = line[col];
            uint32_t a = COLOR_A(color);
            uint8_t r = COLOR_R(color);
            uint8_t g = COLOR_G(color);
            uint8_t b = COLOR_B(color);
            uint8_t lum = calcY(r, g, b);
            switch(t) {
                case ROSE:
                {
                    line[col] = a | (lum << 16) | (g << 8) | b;
                    break;
                }
                case WEIRD:
                {
                    line[col] = a | (r << 16) | (lum << 8) | lum;
                    break;
                }
                case CHILL:
                {
                    uint8_t tint = (b - lum) * intensity;
                    line[col] = a | (r << 16) | (g << 8) | tint;
                    break;
                }
                default:
                {
                    line[col] = a | (lum << 16) | (lum << 8) | lum;
                    break;
                }
            }
        }
        // go to next line
        pixels = (char*)pixels + info->stride;
    }
}

static int checkInfo(JNIEnv* env, jobject bitmap, AndroidBitmapInfo* info)
{
    int ret;
    if ((ret = AndroidBitmap_getInfo(env, bitmap, info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return ret;
    }

    if ((*info).format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGB_8888 ! error=%d", (*info).format);
        return -1;
    }
}

static void filter_bitmap(JNIEnv* env, jobject obj, jobject bitmap,
    float intensity, Filter t)
{
    AndroidBitmapInfo info;
    int ret = checkInfo(env, bitmap, &info);

    if (ret < 0) {
        return;
    }

    void* pixels;

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    do_filter(&info, pixels, intensity, t);

    AndroidBitmap_unlockPixels(env, bitmap);
}

JNIEXPORT void JNICALL Java_com_vandalsoftware_filter_Filter_filterBW
    (JNIEnv* env, jobject obj, jobject bitmap)
{
    filter_bitmap(env, obj, bitmap, 0, NONE);
}

JNIEXPORT void JNICALL Java_com_vandalsoftware_filter_Filter_tintBlue
    (JNIEnv* env, jobject obj, jobject bitmap, jfloat intensity)
{
    filter_bitmap(env, obj, bitmap, intensity, CHILL);
}

JNIEXPORT void JNICALL Java_com_vandalsoftware_filter_Filter_tintRed
    (JNIEnv* env, jobject obj, jobject bitmap, jfloat intensity)
{
    filter_bitmap(env, obj, bitmap, intensity, ROSE);
}
