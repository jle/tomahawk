package com.vandalsoftware.filter;

import android.graphics.Bitmap;

public class Filter {
    static {
        System.loadLibrary("filter");
    }

    private Filter() {
    }

    public static native void filterBW(Bitmap bitmap);
}
