package com.afeskov.defironsource;

import android.app.Activity;

import com.ironsource.mediationsdk.IronSource;
import com.ironsource.mediationsdk.integration.IntegrationHelper;
import com.ironsource.mediationsdk.logger.IronSourceError;
import com.ironsource.mediationsdk.model.Placement;
import com.ironsource.mediationsdk.sdk.InterstitialListener;
import com.ironsource.mediationsdk.sdk.RewardedVideoListener;

public class IronSourceWrapper {

    private Activity activity;
    private RewardedVideoListener rewardedVideoListener;
    private InterstitialListener interstitialListener;

    public IronSourceWrapper(Activity appActivity) {
        activity = appActivity;
        rewardedVideoListener = new StubRewardedVideoListener();
        interstitialListener = new StubInterstitialListener();
    }

    public void init(String appKey) {
        IronSource.init(activity, appKey);
        IronSource.setRewardedVideoListener(rewardedVideoListener);
        IronSource.setInterstitialListener(interstitialListener);
    }

    public void onPause() {
        IronSource.onPause(activity);
    }

    public void onResume() {
        IronSource.onResume(activity);
    }

    public void validateIntegration() {
        IntegrationHelper.validateIntegration(activity);
    }

    private class StubRewardedVideoListener implements RewardedVideoListener {

        @Override
        public void onRewardedVideoAdOpened() {

        }

        @Override
        public void onRewardedVideoAdClosed() {

        }

        @Override
        public void onRewardedVideoAvailabilityChanged(boolean b) {

        }

        @Override
        public void onRewardedVideoAdStarted() {

        }

        @Override
        public void onRewardedVideoAdEnded() {

        }

        @Override
        public void onRewardedVideoAdRewarded(Placement placement) {

        }

        @Override
        public void onRewardedVideoAdShowFailed(IronSourceError ironSourceError) {

        }

        @Override
        public void onRewardedVideoAdClicked(Placement placement) {

        }
    }

    private class StubInterstitialListener implements InterstitialListener {

        @Override
        public void onInterstitialAdReady() {

        }

        @Override
        public void onInterstitialAdLoadFailed(IronSourceError ironSourceError) {

        }

        @Override
        public void onInterstitialAdOpened() {

        }

        @Override
        public void onInterstitialAdClosed() {

        }

        @Override
        public void onInterstitialAdShowSucceeded() {

        }

        @Override
        public void onInterstitialAdShowFailed(IronSourceError ironSourceError) {

        }

        @Override
        public void onInterstitialAdClicked() {

        }
    }
}
