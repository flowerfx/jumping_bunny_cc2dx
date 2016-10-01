using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using VMAXSampleAppWP8.Resources;
using com.vserv.windows.ads.wp8;
using com.vserv.windows.ads;
using Coding4Fun.Toolkit.Controls;

namespace VMAXSampleAppWP8
{
    public partial class MainPage : PhoneApplicationPage
    {
        private const string interstitialTestZoneId = "8063";//"8063";
        private const string bannerTestZoneId = "8042561e";

        public MainPage()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Button clickedButton = sender as Button;
            var buttonTag = clickedButton.Tag as string;
            
            switch (buttonTag)
            {
                case "1":
                    LoadInterstitialDynamically();
                    break;
                case "2":
                    LoadInterstitialThroughXAML();
                    break;
                case "3":
                    CacheAndShowInterstitial();
                    break;
                case "4":
                    LoadInterstitialWithOrientation();
                    break;
                case"5":
                    LoadBannerDynamically();
                    break;
                case "6":
                    LoadBannerThroughXAML();
                    break;
                case "7":
                    CacheAndShowBanner();
                    break;
                default:
                    break;
            }
        }

        private void CacheAndShowBanner()
        {
            SetAdViewGridVisibility(true);

            adView.UX = VservAdUX.Banner;
            adView.ZoneId = bannerTestZoneId;
            adView.Refresh = false;
            adView.CacheAd();
        }

        private void LoadBannerThroughXAML()
        {
            SetAdViewGridVisibility(true);
            adView.UX = VservAdUX.Banner;
            adView.ZoneId = bannerTestZoneId;
            adView.Refresh = false;
            adView.LoadAd();
        }

        private void LoadBannerDynamically()
        {
            SetAdViewGridVisibility(false);

            VservAdView bannerAdView = new VservAdView();
            bannerAdView.ZoneId = bannerTestZoneId;  // Enter your <zone id> here 
            bannerAdView.UX = VservAdUX.Banner;
            bannerAdView.Height = 90;
            bannerAdView.Refresh = false;
            bannerAdView.VerticalAlignment = VerticalAlignment.Bottom;
            if (DynamicAdViewGrid.Children.Count>0)
            {
                DynamicAdViewGrid.Children.Clear();
            }
            DynamicAdViewGrid.Children.Add(bannerAdView);
            
            bannerAdView.LoadAd();
        }

        private void LoadInterstitialWithOrientation()
        {
            adView.UX = VservAdUX.Interstitial;
            adView.ZoneId = interstitialTestZoneId;
            adView.Refresh = false;
            adView.RequestedOrientation = RequestedOrientation.Landscape;
            adView.LoadAd();
        }

        private void CacheAndShowInterstitial()
        {
            adView.UX = VservAdUX.Interstitial;
            adView.ZoneId = interstitialTestZoneId;
            adView.Refresh = false;
            adView.CacheAd();
        }

        private void LoadInterstitialThroughXAML()
        {
            adView.UX = VservAdUX.Interstitial;
            adView.ZoneId = interstitialTestZoneId;
            adView.Refresh = false;
            adView.LoadAd();
        }

        private void LoadInterstitialDynamically()
        {
            //SetAdViewGridVisibility(false);

            VservAdView interstitialAdView = new VservAdView();
            interstitialAdView.ZoneId = interstitialTestZoneId;  // Enter your <zone id>   here 
            interstitialAdView.UX = VservAdUX.Interstitial;
            interstitialAdView.Refresh = false;
            interstitialAdView.DidLoadAd += AdReceived;
            interstitialAdView.DidCacheAd += AdCached;
            interstitialAdView.DidInteractWithAd += InteractedWithAd;
            interstitialAdView.FailedToCacheAd += FailedToCacheAd;
            interstitialAdView.FailedToLoadAd += AdFailed;
            interstitialAdView.WillDismissOverlay += AdCollapsed;
            
            //DynamicAdViewGrid.Children.Add(interstitialAdView); //commented to check the fix of issue caused in case of interstitial ad loaded without putting it in a container .
            interstitialAdView.LoadAd();
        }

        private void SetAdViewGridVisibility(bool visibility)
        {
            this.AdViewGrid.Visibility = visibility ? Visibility.Visible : Visibility.Collapsed;
            this.DynamicAdViewGrid.Visibility = (!visibility) ? Visibility.Visible : Visibility.Collapsed;
        }

        private void AdCached(object sender, EventArgs e)
        {
            ShowToastMesage("DidCacheAd");
            adView.ShowAd();
        }

        private void AdReceived(object sender, EventArgs e)
        {
            ShowToastMesage("DidLoadAd");
        }

        private void AdFailed(object sender, VservAdView.AdFailedEventArgs e)
        {
            ShowToastMesage("FailedToLoadAd");
        }

        private void InteractedWithAd(object sender, EventArgs e)
        {
            ShowToastMesage("DidInteractWithAd");
        }

        private void AdCollapsed(object sender, EventArgs e)
        {
            ShowToastMesage("WillDismissOverlay");
            if (DynamicAdViewGrid.Children.Count > 0)
            {
                DynamicAdViewGrid.Children.Clear();
            }
            DynamicAdViewGrid.Visibility = Visibility.Collapsed;
        }

        private void LeavingApplication(object sender, EventArgs e)
        {
            ShowToastMesage("WillLeaveApp");
        }

        private void AdExpanded(object sender, EventArgs e)
        {
            ShowToastMesage("WillPresentOverlay");
        }

        private void FailedToCacheAd(object sender, EventArgs e)
        {
            ShowToastMesage("FailedToCacheAd");
        }

        private void ShowToastMesage(string message)
        {
            Dispatcher.BeginInvoke(() =>
            {
                ToastPrompt toast = new ToastPrompt();
                toast.Message = message;
                Canvas.SetZIndex(toast, 1);
                toast.Show();
            });
        }
    }
}