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

    public static native void onRewardedVideoAdOpened();
    public static native void onRewardedVideoAdClosed();
    public static native void onRewardedVideoAvailabilityChanged(boolean available);
    public static native void onRewardedVideoAdStarted();
    public static native void onRewardedVideoAdEnded();
    public static native void onRewardedVideoAdRewarded(int placementId, String placementName);
    public static native void onRewardedVideoAdShowFailed(int error, String message);
    public static native void onRewardedVideoAdClicked(int placementId, String placementName);

    public static native void onInterstitialAdReady();
    public static native void onInterstitialAdLoadFailed(int error, String message);
    public static native void onInterstitialAdOpened();
    public static native void onInterstitialAdClosed();
    public static native void onInterstitialAdShowSucceeded();
    public static native void onInterstitialAdShowFailed(int error, String message);
    public static native void onInterstitialAdClicked();

    public IronSourceWrapper(Activity appActivity) {
        activity = appActivity;
        rewardedVideoListener = new DefRewardedVideoListener();
        interstitialListener = new DefInterstitialListener();
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

    public void setConsent(boolean consent) {
        IronSource.setConsent(consent);
    }

    public void loadInterstitial() {
        IronSource.loadInterstitial();
    }

    public boolean isInterstitialReady() {
        return IronSource.isInterstitialReady();
    }

    public void showInterstitial(String placementName) {
        if (placementName != null) {
            IronSource.showInterstitial(placementName);
        } else {
            IronSource.showInterstitial();
        }
    }

    public boolean isRewardedReady() {
        return IronSource.isRewardedVideoAvailable();
    }

    public void showRewarded(String placementName) {
        if (placementName != null) {
            IronSource.showRewardedVideo(placementName);
        } else {
            IronSource.showRewardedVideo();
        }
    }

    private class DefRewardedVideoListener implements RewardedVideoListener {

        @Override
        public void onRewardedVideoAdOpened() {
            IronSourceWrapper.onRewardedVideoAdOpened();
        }

        @Override
        public void onRewardedVideoAdClosed() {
            IronSourceWrapper.onRewardedVideoAdClosed();
        }

        @Override
        public void onRewardedVideoAvailabilityChanged(boolean b) {
            IronSourceWrapper.onRewardedVideoAvailabilityChanged(b);
        }

        @Override
        public void onRewardedVideoAdStarted() {
            IronSourceWrapper.onRewardedVideoAdStarted();
        }

        @Override
        public void onRewardedVideoAdEnded() {
            IronSourceWrapper.onRewardedVideoAdEnded();
        }

        @Override
        public void onRewardedVideoAdRewarded(Placement placement) {
            int placementId = placement.getPlacementId();
            String placementName = placement.getPlacementName();
            IronSourceWrapper.onRewardedVideoAdRewarded(placementId, placementName);
        }

        @Override
        public void onRewardedVideoAdShowFailed(IronSourceError ironSourceError) {
            int error = ironSourceError.getErrorCode();
            String message = ironSourceError.getErrorMessage();
            IronSourceWrapper.onRewardedVideoAdShowFailed(error, message);
        }

        @Override
        public void onRewardedVideoAdClicked(Placement placement) {
            int placementId = placement.getPlacementId();
            String placementName = placement.getPlacementName();
            IronSourceWrapper.onRewardedVideoAdClicked(placementId, placementName);
        }
    }

    private class DefInterstitialListener implements InterstitialListener {

        @Override
        public void onInterstitialAdReady() {
            IronSourceWrapper.onInterstitialAdReady();
        }

        @Override
        public void onInterstitialAdLoadFailed(IronSourceError ironSourceError) {
            int error = ironSourceError.getErrorCode();
            String message = ironSourceError.getErrorMessage();
            IronSourceWrapper.onInterstitialAdLoadFailed(error, message);
        }

        @Override
        public void onInterstitialAdOpened() {
            IronSourceWrapper.onInterstitialAdOpened();
        }

        @Override
        public void onInterstitialAdClosed() {
            IronSourceWrapper.onInterstitialAdClosed();
        }

        @Override
        public void onInterstitialAdShowSucceeded() {
            IronSourceWrapper.onInterstitialAdShowSucceeded();
        }

        @Override
        public void onInterstitialAdShowFailed(IronSourceError ironSourceError) {
            int error = ironSourceError.getErrorCode();
            String message = ironSourceError.getErrorMessage();
            IronSourceWrapper.onInterstitialAdShowFailed(error, message);
        }

        @Override
        public void onInterstitialAdClicked() {
            IronSourceWrapper.onInterstitialAdClicked();
        }
    }
}
