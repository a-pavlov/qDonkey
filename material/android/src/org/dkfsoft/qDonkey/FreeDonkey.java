/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtAndroidExtras module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

package org.dkfsoft.qDonkey;

import android.os.Bundle;
import android.util.Log;
import org.qtproject.qt5.android.bindings.QtActivity;
import org.qtproject.qt5.android.bindings.QtApplication;
import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;
import static com.google.android.gms.ads.AdSize.BANNER;

import java.util.concurrent.atomic.AtomicBoolean;

public class FreeDonkey extends QtActivity
{
    private static NotificationManager m_notificationManager;
    private static Notification.Builder m_builder;
    private static FreeDonkey m_instance;

    private AdView mAdView;
    private InterstitialAd mInterstitialAd;
    private boolean showInterstitial = true;
    private AtomicBoolean adLoaded = new AtomicBoolean(false);

    private int statusBarHeight() {
        int result = 0;
        int resourceId = getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result = getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }

    public FreeDonkey()
    {
        m_instance = this;
    }

    public static void notify(String s)
    {
        if (m_notificationManager == null) {
            m_notificationManager = (NotificationManager)m_instance.getSystemService(Context.NOTIFICATION_SERVICE);
            m_builder = new Notification.Builder(m_instance);
            m_builder.setSmallIcon(R.drawable.icon);
            m_builder.setContentTitle("qDonkey");
        }

        m_builder.setContentText(s);
        m_notificationManager.notify(1, m_builder.build());
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mAdView = new AdView(this);
        mAdView.setAdSize(BANNER);
        mAdView.setAdUnitId(getString(R.string.banner_ad_unit_id));
        mAdView.setVisibility(View.VISIBLE);

        View view = getWindow().getDecorView().getRootView();
        if (view instanceof ViewGroup)
        {
            ViewGroup wg = (ViewGroup) view;
            FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                    FrameLayout.LayoutParams.WRAP_CONTENT);
            mAdView.setLayoutParams(layoutParams);
            mAdView.setX(0);
            mAdView.setY(statusBarHeight());
            wg.addView(mAdView);
        }

        // Create an ad request. Check your logcat output for the hashed device ID to
        // get test ads on a physical device. e.g.
        // "Use AdRequest.Builder.addTestDevice("ABCDEF012345") to get test ads on this device."
        AdRequest adRequest = new AdRequest.Builder()
                .addTestDevice("0A0D71A15744A91F735BBCA779B2DB81")
                .build();

        mAdView.setAdListener(new AdListener() {
            @Override
            public void onAdClosed() {
                super.onAdClosed();
            }

            @Override
            public void onAdLoaded() {
                super.onAdLoaded();
                adLoaded.set(true);
            }
        });
        // Start loading the ad in the background.
        mAdView.loadAd(adRequest);

        // Create the InterstitialAd and set the adUnitId.
        mInterstitialAd = new InterstitialAd(this);
        // Defined in res/values/strings.xml
        mInterstitialAd.setAdUnitId(getString(R.string.ad_unit_id));
        mInterstitialAd.loadAd(new AdRequest.Builder().addTestDevice("0A0D71A15744A91F735BBCA779B2DB81").build());

        mInterstitialAd.setAdListener(new AdListener() {
            @Override
            public void onAdClosed() {
                Log.d("Interstitial", "Load AD again");
                mInterstitialAd.loadAd(new AdRequest.Builder().addTestDevice("0A0D71A15744A91F735BBCA779B2DB81").build());
            }

            @Override
            public void onAdLoaded() {
                super.onAdLoaded();
                Log.d("Interstitial", "Loaded");
                if (showInterstitial) {
                    showInterstitialAd();
                }
            }

        });
    }

    @Override
    public void onPause() {
        if (mAdView != null) {
            mAdView.pause();
        }
        super.onPause();
    }

    /** Called when returning to the activity */
    @Override
    public void onResume() {
        super.onResume();
        if (mAdView != null) {
            mAdView.resume();
        }
    }

    /** Called before the activity is destroyed */
    @Override
    public void onDestroy() {
        if (mAdView != null) {
            mAdView.destroy();
        }
        super.onDestroy();
    }


    public void showInterstitialAd() {
        if (mInterstitialAd.isLoaded()) {
            showInterstitial = false;
            mInterstitialAd.show();
        }
    }

    public boolean adLoaded() {
        return adLoaded.get();
    }

    public void adShow() {
        runOnUiThread(new Runnable() {
            public void run() {
                    Log.d("adMob", "adVisible");
                    mAdView.setVisibility(View.VISIBLE);
                }
            });
    }

    public void adHide() {
        runOnUiThread(new Runnable() {
            public void run() {
                    Log.d("adMob", "adInvisible");
                    mAdView.setVisibility(View.INVISIBLE);
                }
            });
    }

    public void adSetPosition(final int x, final int y) {
        runOnUiThread(new Runnable() {
            public void run() {
                    Log.d("adMob", "set pos");
                    FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                                                                                         FrameLayout.LayoutParams.WRAP_CONTENT);
                    mAdView.setLayoutParams(layoutParams);
                    mAdView.setX(x);
                    mAdView.setY(y + statusBarHeight());
                }
            });
    }

    public int adHeight() {
        return mAdView.getHeight();
    }

    public void interstitialShow() {
        runOnUiThread(new Runnable() {
            public void run() {
                Log.d("adMob", "show interstitial");
                showInterstitial = true;
                showInterstitialAd();
            }
        });
    }

}
