package com.vandalsoftware.filter;

import android.app.Fragment;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import java.io.Closeable;
import java.io.IOException;
import java.io.InputStream;

/**
 * A simple fragment that displays a single full-screen photograph.
 *
 * @author Jonathan Le
 */
public class PhotoFragment extends Fragment {
    public static final String ARG_FILTER = "f";
    public static final int FILTER_NONE = 0;
    public static final int FILTER_CHILL = 1;
    public static final int FILTER_ROSE = 2;
    public static final int FILTER_BW = 3;
    private int mFilter;

    public PhotoFragment() {
        super();
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Bundle bundle = getArguments();
        mFilter = bundle.getInt(ARG_FILTER, FILTER_NONE);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        return inflater.inflate(R.layout.photo, container, false);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        InputStream is = null;
        try {
            is = getResources().getAssets().open("Lenna.png");

            final BitmapFactory.Options opts = new BitmapFactory.Options();
            opts.inPurgeable = true;
            opts.inMutable = true;

            final Bitmap bm = BitmapFactory.decodeStream(is, null, opts);
            switch (mFilter) {
                case FILTER_CHILL: {
                    Filter.tintBlue(bm, 1);
                    break;
                }
                case FILTER_ROSE: {
                    Filter.tintRed(bm, .75f);
                    break;
                }
                case FILTER_BW: {
                    Filter.filterBW(bm);
                    break;
                }
            }
            final ImageView imageView = (ImageView) view;
            imageView.setImageBitmap(bm);
        } catch (IOException ignored) {
        } finally {
            closeQuietly(is);
        }
    }

    private void closeQuietly(Closeable c) {
        if (c != null) {
            try {
                c.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
