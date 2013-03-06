package com.vandalsoftware.filter;

import android.graphics.Bitmap;

public class Filter {
    static {
        System.loadLibrary("filter");
    }

    private Filter() {
    }

    public static native void filterBW(Bitmap bitmap);
    public static native void tintBlue(Bitmap bitmap, float intensity);
    public static native void tintRed(Bitmap bitmap, float intensity);
}
