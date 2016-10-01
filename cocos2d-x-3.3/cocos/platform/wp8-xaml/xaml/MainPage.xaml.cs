#if DEBUG
#define DISPLAY_MEMORY
#endif

using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using System.Threading;
using System.Diagnostics;
using System.Threading.Tasks;
using Microsoft.Phone.Shell;
using Windows.UI.Input;
using System.Windows.Threading;
using Microsoft.Phone.Info;
using Windows.Graphics.Display;
using Microsoft.Phone.Tasks;
using Microsoft.Phone.Net.NetworkInformation;

using Facebook;
using Facebook.Client;

using CCFacebookUtils;
using System.Text.RegularExpressions;

#region IAP
#if DEBUG
using MockIAPLib;
using Store = MockIAPLib;
#else
using Windows.ApplicationModel.Store;
using Windows.ApplicationModel;

using Store = Windows.ApplicationModel.Store;
#endif
#endregion

#region ADS
using com.vserv.windows.ads.wp8;
using com.vserv.windows.ads;
#endregion
namespace cocos2d
{
    public enum InternetConnectionType
    {
        NoConnection = 0,
        Mobile2GConnection,
        Mobile3GConnection,
        WirelessConnection,
        Mobile4GConnection,
        UnknownConnection
    };

    public partial class MainPage : PhoneApplicationPage
    {
        private const string bannerTestZoneId = "8042561e";
        bool IsAdsReceiving = false;

        private Direct3DInterop m_d3dInterop = null;
        private DispatcherTimer m_timer;

        private string deviceID= "";
        // event handler for CCEditBox
        private event EventHandler<String> m_receiveHandler;
        //timer to show logo
        System.Windows.Threading.DispatcherTimer fxLogoTimer = new System.Windows.Threading.DispatcherTimer();
        // invisible XAML TextBox for Cocos2d-x keyboard input
        TextBox m_textBox = null;
        //
        private bool p_IsOnLogoState = true;
        #region IAP
        int m_StateGetAllItemFromStoreSuccess = 0; // 0 is no loading, 1 is loading, 2 is load succeed . -1 is load fail
        int m_StatePurchaseItemFromStore = 0; // 0 is no purchase, 1 is processing, 2 is purchase succeed, -1 is purchase fail
        bool m_IsRemoveAds = false;
        string m_PriceValue = "";
        bool m_isPanelShareFBAppear = false;
        #endregion

        bool m_isClosePopUp = false;
        // Constructor
        public MainPage()
        {
            InitializeComponent();
#if DISPLAY_MEMORY
            StartTimer();
#else
            MemoryDisplay.Visibility = Visibility.Collapsed;
#endif
            byte[] id = (byte[])Microsoft.Phone.Info.DeviceExtendedProperties.GetValue("DeviceUniqueId");
            deviceID = Convert.ToBase64String(id);
            //
            fxLogoTimer.Interval = TimeSpan.FromSeconds(2);
            fxLogoTimer.Tick += new EventHandler(FXLogoTimeout);
            fxLogoTimer.Start();
        }

        private void FXLogoTimeout(object sender, EventArgs e)
        {
            this.Dispatcher.BeginInvoke(
            delegate()
            {
                if (RootLayout.Children.Contains(FFXLogo))
                    RootLayout.Children.Remove(FFXLogo);
            });
            p_IsOnLogoState = false;
        }

        public bool GetLogoState()
        {
            return p_IsOnLogoState;
        }

        override protected void OnOrientationChanged(OrientationChangedEventArgs args)
        {
            base.OnOrientationChanged(args);
            if (m_d3dInterop != null)
            {
                DisplayOrientations orientation = ConvertToNativeOrientation(args.Orientation);
                m_d3dInterop.OnOrientationChanged(orientation);
            }
        }

        private static DisplayOrientations ConvertToNativeOrientation(PageOrientation xamlOrientation)
        {
            switch (xamlOrientation)
            {
                case PageOrientation.Portrait:
                case PageOrientation.PortraitUp:
                    return DisplayOrientations.Portrait;
                case PageOrientation.PortraitDown:
                    return DisplayOrientations.PortraitFlipped;
                case PageOrientation.Landscape:
                case PageOrientation.LandscapeLeft:
                    return DisplayOrientations.Landscape;
                case PageOrientation.LandscapeRight:
                    return DisplayOrientations.LandscapeFlipped;
                default:
                    return DisplayOrientations.Landscape;
            }
        }

        private void DrawingSurfaceBackground_Loaded(object sender, RoutedEventArgs e)
        {
            if (m_d3dInterop == null)
            {
                PageOrientation pageOrientation = (PageOrientation)GetValue(OrientationProperty);
                DisplayOrientations displayOrientation = ConvertToNativeOrientation(pageOrientation);

                m_d3dInterop = new Direct3DInterop(displayOrientation);

                // Set WindowBounds to size of DrawingSurface
                m_d3dInterop.WindowBounds = new Windows.Foundation.Size(
                    (float)Application.Current.Host.Content.ActualWidth,
                    (float)Application.Current.Host.Content.ActualHeight
                    );
                // Set native resolution in pixels
                m_d3dInterop.NativeResolution = new Windows.Foundation.Size(
                    (float)Math.Floor(Application.Current.Host.Content.ActualWidth * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f),
                    (float)Math.Floor(Application.Current.Host.Content.ActualHeight * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f)
                    );

                // Set render resolution to the full native resolution
                m_d3dInterop.RenderResolution = m_d3dInterop.NativeResolution;

                // Hook-up native component to DrawingSurfaceBackgroundGrid
                DrawingSurfaceBackground.SetBackgroundContentProvider(m_d3dInterop.CreateContentProvider());
                DrawingSurfaceBackground.SetBackgroundManipulationHandler(m_d3dInterop);

                // Hook-up Cocos2d-x delegates
                m_d3dInterop.SetCocos2dEventDelegate(OnCocos2dEvent);
                m_d3dInterop.SetCocos2dMessageBoxDelegate(OnCocos2dMessageBoxEvent);
                m_d3dInterop.SetCocos2dEditBoxDelegate(OpenEditBox);
                m_d3dInterop.SetCocos2dOpenURLDelegate(OpenURL);

                cocos2d.Direct3DInterop.m_openFacebookPage += NavigateConnectPage;
                cocos2d.Direct3DInterop.m_ExitApp += OnExitApp;
                cocos2d.Direct3DInterop.m_Get_DeviceID += GetDeviceID;
                cocos2d.Direct3DInterop.m_Get_LogoState += GetLogoState;
                #region FB
                cocos2d.Direct3DInterop.m_FB_GetToken += GetFacebookToken;
                cocos2d.Direct3DInterop.m_FB_GetID += GetFacebookID;
                cocos2d.Direct3DInterop.m_FB_GetName += GetFacebookName;
                cocos2d.Direct3DInterop.m_FB_GetImgURL += GetFacebookProfileImgURL;
                cocos2d.Direct3DInterop.m_FB_GetImgURLfromID += GetFBProfileImgFromID;
                cocos2d.Direct3DInterop.m_FB_GetScore += GetUserScore;
                cocos2d.Direct3DInterop.m_FB_RequestScore += RequestUserScore;
                cocos2d.Direct3DInterop.m_FB_UpdateScore += UpdateUserScore;
                cocos2d.Direct3DInterop.m_FB_ShareFB += ShareFB;
                cocos2d.Direct3DInterop.m_FB_SharePanelAppear += IsFBSharePanelAppear;
                cocos2d.Direct3DInterop.m_FB_CloseSharePanel += CloseSharePanelFB;

                this.fb_share_btn.Click += fb_share_btn_action;
                this.fb_cancel_btn.Click += fb_cancel_btn_action;
                this.fb_TextBox.LostFocus += fb_panel_lost_focus;
                #endregion
                cocos2d.Direct3DInterop.m_IsHaveConnection += IsHaveConnection;
                cocos2d.Direct3DInterop.m_RateGame += OnRateTheGame;
                #region IAP
                cocos2d.Direct3DInterop.m_IAP_GetStateLoadItem += GetStateLoadListItemFromStore;
                cocos2d.Direct3DInterop.m_IAP_GetStatePurchaseItem += GetStatePurchaseItemFromStore;
                cocos2d.Direct3DInterop.m_IAP_SetStateLoadItem += SetStateLoadListItemFromStore;
                cocos2d.Direct3DInterop.m_IAP_SetStatePurchaseItem += SetStatePurchaseItemFromStore;

                cocos2d.Direct3DInterop.m_IAP_PurchaseItemWithID += OnPurchaseItem;
                cocos2d.Direct3DInterop.m_IAP_InitStoreItem += InitTheItemStore;
                cocos2d.Direct3DInterop.m_IAP_RequestListItem += GetListItemFromStore;
                cocos2d.Direct3DInterop.m_IAP_DoFulfillmentItemId += DoFulfillmentItem;

                cocos2d.Direct3DInterop.m_IAP_CheckFulfillmentIfNotFulfill += FullfillItemsIfUnfulfilled;
                cocos2d.Direct3DInterop.m_IAP_IsRemoveAds += IsRemoveAds;

                cocos2d.Direct3DInterop.m_IAP_GetPriceValue += GetPriceValue;   
                #endregion
                cocos2d.Direct3DInterop.m_PopUp_GetClose += GetClosePopUp;
                cocos2d.Direct3DInterop.m_PopUp_SetClose += SetClosePopUp;
                #region ADS
                cocos2d.Direct3DInterop.m_Ads_ShowBanner += ShowBannerAds;
                cocos2d.Direct3DInterop.m_Ads_HideBanner += HideAdsBanner;
                cocos2d.Direct3DInterop.m_Ads_IsAppearing += IsBannerAppearing;
                this.___CloseAdsBtn_.Click += ___CloseAdsBtn__Click;
                #endregion

                //login-fb by the saved token
                if (CCFacebookUtils.FacebookClient.Instance.AccessToken != "" && GetInternetConnectionInfo()!= InternetConnectionType.NoConnection )
                {
                    CCFacebookUtils.FacebookClient.Instance.GetFacebookProfile(CCFacebookUtils.FacebookClient.Instance.AccessToken);
                }
            }
        }

        // called when the user presses the back button on the device
        protected override void OnBackKeyPress(CancelEventArgs e)
        {
            m_d3dInterop.OnBackKeyPress();
            // cocos2d-x will async send Cocos2dEvent.TerminateApp event if it is time to exit app.
            // We do not want to exit now, so we set e.Cancel to true.
            e.Cancel = true;
        }

        public void OnKeyDown(object sender, KeyEventArgs e)
        {
            ModifierKeys modifiers = Keyboard.Modifiers;

            switch (e.Key)
            {
                case Key.Escape:
                    m_d3dInterop.OnCocos2dKeyEvent(Cocos2dKeyEvent.Escape);
                    e.Handled = true;
                    break;
                case Key.Back:
                    m_d3dInterop.OnCocos2dKeyEvent(Cocos2dKeyEvent.Back);
                    e.Handled = true;
                    break;
                case Key.Enter:
                    m_d3dInterop.OnCocos2dKeyEvent(Cocos2dKeyEvent.Enter);
                    e.Handled = true;
                    break;
                default:
                    break;
            }
        }

        public void OnKeyUp(object sender, KeyEventArgs e)
        {
        }

        public void OnTextChanged(object sender, TextChangedEventArgs e)
        {
            if (m_textBox.Text.Length > 0)
            {
                m_d3dInterop.OnCocos2dKeyEvent(Cocos2dKeyEvent.Text, m_textBox.Text);
                m_textBox.Text = "";
            }
        }

        // Called by the Cocos2d-x C++ engine to display a MessageBox
        public void OnCocos2dMessageBoxEvent(String title, String text)
        {
            Dispatcher.BeginInvoke(() =>
            {
                MessageBox.Show(text, title, MessageBoxButton.OK);
            });
        }

        // events called by the Cocos2d-x C++ engine to be handled by C#
        public void OnCocos2dEvent(Cocos2dEvent theEvent, String text)
        {
            Dispatcher.BeginInvoke(() =>
            {
                switch (theEvent)
                {
                    case Cocos2dEvent.TerminateApp:
                        Application.Current.Terminate();
                        break;

                    case Cocos2dEvent.ShowKeyboard:
                        if (m_textBox == null)
                        {
                            m_textBox = new TextBox();
                            m_textBox.Opacity = 0.0;
                            m_textBox.Width = 1;
                            m_textBox.Height = 1;
                            m_textBox.KeyDown += OnKeyDown;
                            m_textBox.TextChanged += OnTextChanged;
                            DrawingSurfaceBackground.Children.Add(m_textBox);
                        }
                        m_textBox.SelectionLength = 0;
                        m_textBox.SelectionStart = int.MaxValue;
                        m_textBox.Focus();
                        break;

                    case Cocos2dEvent.HideKeyboard:
                        if (m_textBox != null)
                        {
                            DrawingSurfaceBackground.Children.Remove(m_textBox);
                        }
                        m_textBox = null;
                        break;
                }
            });
        }

        // Called by the Cocos2d-x C++ engine to display a CCEditBox
        public void OpenEditBox(String strPlaceHolder, string strText, int maxLength, int inputMode, int inputFlag, EventHandler<String> receiveHandler)
        {
            m_receiveHandler = receiveHandler;
            Deployment.Current.Dispatcher.BeginInvoke(() =>
            {
                EditBox editbox = new EditBox(this, strPlaceHolder, strText, maxLength, inputMode, inputFlag);
                DrawingSurfaceBackground.Children.Add(editbox);
            });
        }

        public void OnSelectText(object sender, String str)
        {
            if (m_d3dInterop != null && m_receiveHandler != null)
            {
                m_d3dInterop.OnCocos2dEditboxEvent(sender, str, m_receiveHandler);
            }
        }

        public void OpenURL(String url)
        {
            WebBrowserTask webBrowserTask = new WebBrowserTask();
            webBrowserTask.Uri = new Uri(url, UriKind.Absolute);
            webBrowserTask.Show();
        }

        private void StartTimer()
        {
            m_timer = new DispatcherTimer();
            m_timer.Interval = new TimeSpan(0, 0, 1);
            m_timer.Tick += new EventHandler(TimerTick);
            m_timer.Start();
        }

        private void TimerTick(object sender, EventArgs e)
        {
            try
            {
                // These are TextBlock controls that are created in the page’s XAML file.  
                float value = DeviceStatus.ApplicationCurrentMemoryUsage / (1024.0f * 1024.0f);
                MemoryTextBlock.Text = value.ToString();
                value = DeviceStatus.ApplicationPeakMemoryUsage / (1024.0f * 1024.0f);
                PeakMemoryTextBlock.Text = value.ToString();
            }
            catch (Exception ex)
            {
                MemoryTextBlock.Text = ex.Message;
            }
        }
        public void OnExitApp()
        {
            App.Current.Terminate();
        }

        public string GetDeviceID()
        {
            return deviceID;
        }

        private void OnRateTheGame()
        {
           Dispatcher.BeginInvoke(() =>
           {
               MarketplaceReviewTask marketplaceReviewTask = new MarketplaceReviewTask();

               marketplaceReviewTask.Show();
           });
        }

        #region FACEBOOK
        public void NavigateConnectPage()
        {
            if(GetInternetConnectionInfo() == InternetConnectionType.NoConnection)
            {
                return;
            }
            String uriToLaunch = CCFacebookUtils.FacebookClient.Instance.GetLoginUrl();

            var uri = new Uri(uriToLaunch);

            DefaultLaunch(uri);
        }

        async void DefaultLaunch(Uri uri)
        {
            var success = await Windows.System.Launcher.LaunchUriAsync(uri);
        }

        string GetFacebookToken()
        {
            return CCFacebookUtils.FacebookClient.Instance.AccessToken;
        }
        string GetFacebookName()
        {
            return CCFacebookUtils.FacebookClient.Instance.NameFB;
        }

        string GetFacebookID()
        {
            return CCFacebookUtils.FacebookClient.Instance.ID;
        }

        string GetFacebookProfileImgURL()
        {
            string token = CCFacebookUtils.FacebookClient.Instance.AccessToken;
            string ID = CCFacebookUtils.FacebookClient.Instance.ID;
            return CCFacebookUtils.FacebookClient.Instance.GetUserProfilePicture(ID,token);
        }
        string GetFBProfileImgFromID(string id)
        {
            return CCFacebookUtils.FacebookClient.Instance.GetProfilePictureFromID(id);
        }
        void RequestUserScore()
        {
            if(GetInternetConnectionInfo()== InternetConnectionType.NoConnection)
            {
                return;
            }
            CCFacebookUtils.FacebookClient.Instance.RequestUserScore();
        }
        string GetUserScore()
        {
            return CCFacebookUtils.FacebookClient.Instance.GetUserScore();
        }
        void UpdateUserScore(int score)
        {
            if (GetInternetConnectionInfo() == InternetConnectionType.NoConnection)
            {
                return;
            }
            CCFacebookUtils.FacebookClient.Instance.UploadUserScore(score);
        }

        void ShareFB(string str)
        {
            if (GetInternetConnectionInfo() == InternetConnectionType.NoConnection)
            {
                return;
            }
            Dispatcher.BeginInvoke(() =>
                {
                    //display the xaml text box
                    fb_TextBox.Visibility = Visibility.Visible;
                    m_isPanelShareFBAppear = true;
                    //set string
                    this.fb_textbox.Text = str.ToString();
                });
        }
        void fb_share_btn_action(object sender,  EventArgs e)
        {
            string str = this.fb_textbox.Text;
            fb_TextBox.Visibility = Visibility.Collapsed;
            m_isPanelShareFBAppear = false;
            CCFacebookUtils.FacebookClient.Instance.ShareOnFacebook(str);
        }
        void fb_cancel_btn_action(object sender, EventArgs e)
        {
            this.fb_textbox.Text = "";
            fb_TextBox.Visibility = Visibility.Collapsed;
            m_isPanelShareFBAppear = false;
        }
        void fb_panel_lost_focus(object sender, EventArgs e)
        {
            this.fb_textbox.Text = "";
            fb_TextBox.Visibility = Visibility.Collapsed;
            m_isPanelShareFBAppear = false;
        }
        bool IsFBSharePanelAppear()
        {
            return m_isPanelShareFBAppear;
        }
        void CloseSharePanelFB()
        {
             Dispatcher.BeginInvoke(() =>
                {
                    this.fb_textbox.Text = "";
                    fb_TextBox.Visibility = Visibility.Collapsed;
                    m_isPanelShareFBAppear = false;
                });
        }
        #endregion

        #region CheckInternet
        private static InternetConnectionType getConnectionTypeFromNetworkInterce(NetworkInterfaceInfo info)
        {
            if (info != null)
            {
                switch (info.InterfaceSubtype)
                {
                    case NetworkInterfaceSubType.Unknown:
                        return InternetConnectionType.NoConnection;
                    case NetworkInterfaceSubType.Cellular_GPRS:
                        return InternetConnectionType.Mobile2GConnection;
                    case NetworkInterfaceSubType.Cellular_1XRTT:
                        return InternetConnectionType.Mobile2GConnection;
                    case NetworkInterfaceSubType.Cellular_EVDO:
                        return InternetConnectionType.Mobile3GConnection;
                    case NetworkInterfaceSubType.Cellular_EDGE:
                        return InternetConnectionType.Mobile2GConnection;
                    case NetworkInterfaceSubType.Cellular_3G:
                        return InternetConnectionType.Mobile3GConnection;
                    case NetworkInterfaceSubType.Cellular_HSPA:
                        return InternetConnectionType.Mobile3GConnection;
                    case NetworkInterfaceSubType.Cellular_EVDV:
                        return InternetConnectionType.Mobile3GConnection;
                    case NetworkInterfaceSubType.Desktop_PassThru:
                        return InternetConnectionType.UnknownConnection; // for now it's unknown
                    case NetworkInterfaceSubType.WiFi:
                        return InternetConnectionType.WirelessConnection;
                    case NetworkInterfaceSubType.Cellular_LTE:
                        return InternetConnectionType.Mobile4GConnection;
                    case NetworkInterfaceSubType.Cellular_EHRPD:
                        return InternetConnectionType.Mobile4GConnection;
                    default:
                        return InternetConnectionType.UnknownConnection;
                }
            }
            return InternetConnectionType.NoConnection;
        }
        public InternetConnectionType GetInternetConnectionInfo()
        {
            NetworkInterfaceList list = new NetworkInterfaceList();
            InternetConnectionType connectionType;
            connectionType = InternetConnectionType.NoConnection;
            //
            var ni = NetworkInterface.NetworkInterfaceType;
            //
            bool IsConnected = false;
            if ((ni == NetworkInterfaceType.Wireless80211) || (ni == NetworkInterfaceType.MobileBroadbandCdma) || (ni == NetworkInterfaceType.MobileBroadbandGsm))
            {
                IsConnected = true;
                if (ni == NetworkInterfaceType.Wireless80211)
                {
                    connectionType = InternetConnectionType.WirelessConnection;
                }
                else if (ni == NetworkInterfaceType.MobileBroadbandCdma || ni == NetworkInterfaceType.MobileBroadbandGsm)
                {
                    connectionType = InternetConnectionType.Mobile3GConnection;
                }
            }
            else if (ni == NetworkInterfaceType.None)
            {
                IsConnected = false;
                connectionType = InternetConnectionType.NoConnection;
            }
            //
            if (IsConnected == false)
                connectionType = InternetConnectionType.NoConnection;
            return connectionType;
        }

        public bool IsHaveConnection()
        {
            return (GetInternetConnectionInfo() != InternetConnectionType.NoConnection);
        }
        #endregion
        #region IAP
        public async void GetListItemFromStore()
        {
            m_StateGetAllItemFromStoreSuccess = 1;
            List<string> li = await StoreManager.GetInstance().GetOwnedItems();
            bool result = await GetCorrectPriceValue();
            if (li.Count() != StoreManager.GetInstance().StoreItems.Count || !result) //debug
            {
                m_StateGetAllItemFromStoreSuccess = -1;
            }
            else
            {
                if (result)
                {
                    m_StateGetAllItemFromStoreSuccess = 2;
                }
                else
                {
                    m_StateGetAllItemFromStoreSuccess = -1;
                }
            }
            m_isClosePopUp = true;
        }
        public async void OnPurchaseItem(string ItemID)
        {
            if (m_StateGetAllItemFromStoreSuccess != 2)
            {
                return;
            }
            //this call when all the item load succeed
            if (!Store.CurrentApp.LicenseInformation.ProductLicenses[ItemID].IsActive)
            {
                m_StatePurchaseItemFromStore = 1; //processing purchase !
                ListingInformation li = await Store.CurrentApp.LoadListingInformationAsync();
                string pID = ItemID;// li.ProductListings[ItemID].ProductId;
                Dispatcher.BeginInvoke(async delegate() 
                {
                    try
                    {
                        await Store.CurrentApp.RequestProductPurchaseAsync(pID, false);                    
                        m_StatePurchaseItemFromStore = 2; //purchase succeed !                    
                    }
                    catch (Exception ex)
                    {
                        m_StatePurchaseItemFromStore = -1; // purchase fail !
                    }
                    m_isClosePopUp = true; //close popup if it exist
                });
            }
            else
            {
                m_StatePurchaseItemFromStore = -1; // purchase fail !
                m_isClosePopUp = true; //close popup if it exist
            }
        }
        public bool IsRemoveAds()
        {
            return m_IsRemoveAds;
        }
        public void DoFulfillmentItem(string id) //after purchase item, remember to dofulfillment
        {
            // Check fulfillment for consumable products with variable asset counts
            var productLicenses = Store.CurrentApp.LicenseInformation.ProductLicenses;
            if (!productLicenses.ContainsKey(id))
                return;
            if (productLicenses[id].IsConsumable && productLicenses[id].IsActive)
            {
                // Report item fulfilled, so it can be purchased again
                Store.CurrentApp.ReportProductFulfillment(id);
            }
        }
        public string FullfillItemsIfUnfulfilled(string keyGain) // check fulfillment at the init
        {
            List<string> li = new List<string>();
            for(int i = 0 ; i < StoreManager.GetInstance().StoreItems.Count(); i ++)
            {
                var p = StoreManager.GetInstance().StoreItems.ElementAt(i);
                li.Add(p.Key);
            }
            var productLicenses = Store.CurrentApp.LicenseInformation.ProductLicenses;
            keyGain = "";
            for (int i = 0; i < li.Count(); i++)
            {
                var key = li[i];
                if (!productLicenses.ContainsKey(key))
                    continue;
                if (productLicenses[key].IsConsumable && productLicenses[key].IsActive)
                {
                    keyGain += "#" + key;
                    DoFulfillmentItem(key);
                    // and store the data in the app if needed

                }
                else if (productLicenses[key].IsActive && !productLicenses[key].IsConsumable)
                {
                    //remove ads in game
                    m_IsRemoveAds = true;
                    //RemoveAds();
                }
            }
            return keyGain;
 
        }

        public async Task<bool> GetCorrectPriceValue()
        {
            int count = 0;
            string valuePrice = "";
            // get the license info for this app from the game
            try
            {
                ListingInformation listing = await Store.CurrentApp.LoadListingInformationAsync();
                List<string> li = new List<string>();
                for (int i = 0; i < StoreManager.GetInstance().StoreItems.Count(); i++)
                {
                    var p = StoreManager.GetInstance().StoreItems.ElementAt(i);
                    li.Add(p.Key);
                }

                for (int i = 0; i < li.Count(); i++)
                {
                    var key = li[i];
                    if (listing.ProductListings.ContainsKey(key))
                    {
                        // get the ProductListing object for the product named key
                        ProductListing thisProduct = listing.ProductListings[key];
                        valuePrice += "#" + thisProduct.FormattedPrice;
                        count++;
                    }
                }
                m_PriceValue = valuePrice;
                if (count == li.Count())
                {
                    return true;
                }
                return false;
            }
            catch(Exception e)
            {
                return false;
            }
        }
        public void InitTheItemStore(string id, string key)
        {
            StoreManager.GetInstance().InitStoreItem(id, key);
        }
        public int GetStateLoadListItemFromStore()
        {
            return m_StateGetAllItemFromStoreSuccess;
        }
        public int GetStatePurchaseItemFromStore()
        {
            return m_StatePurchaseItemFromStore;
        }
        public void SetStateLoadListItemFromStore(int state)
        {
            m_StateGetAllItemFromStoreSuccess = state;
        }
        public void SetStatePurchaseItemFromStore(int state)
        {
            m_StatePurchaseItemFromStore = state;
        }
        public string GetPriceValue()
        {
            return m_PriceValue;
        }
        #endregion

        public void SetClosePopUp(bool b)
        {
            m_isClosePopUp = b;
        }
        public bool GetClosePopUp()
        {
            return m_isClosePopUp;
        }

        #region ADS
        private void LoadBannerDynamically()
        {
            if (IsAdsReceiving)
                return;
            Dispatcher.BeginInvoke(() =>
                {
                    IsAdsReceiving = true;
                    SetAdViewGridVisibility(true);

                    VservAdView bannerAdView = new VservAdView();
                    bannerAdView.ZoneId = bannerTestZoneId;  // Enter your <zone id> here 
                    bannerAdView.UX = VservAdUX.Banner;
                    bannerAdView.Height = 156;
                    bannerAdView.Refresh = false;
                    bannerAdView.VerticalAlignment = VerticalAlignment.Bottom | VerticalAlignment.Stretch;
                    bannerAdView.DidLoadAd += AdReceived;
                    bannerAdView.DidInteractWithAd += InteractedWithAd;
                    bannerAdView.WillDismissOverlay += AdCollapsed;
                    //
                   // bannerAdView.Margin = new Thickness(150, 147, 150, 0);
                    //
                    if (DynamicAdViewGrid.Children.Count > 0)
                    {
                        DynamicAdViewGrid.Children.Clear();
                    }
                    DynamicAdViewGrid.Children.Add(bannerAdView);
                    bannerAdView.LoadAd();
                });
        }
        
        
        private void SetAdViewGridVisibility(bool visibility) // remember to run this in another thread
        {
            this.DynamicAdViewGrid.Visibility = (!visibility) ? Visibility.Visible : Visibility.Collapsed;
            this.___CloseAdsBtn_.Visibility = (!visibility) ? Visibility.Visible : Visibility.Collapsed;
            this.___ImgBanner_.Visibility = (!visibility) ? Visibility.Visible : Visibility.Collapsed;
        }
        public void HideAdsBanner()
        {
            Dispatcher.BeginInvoke(() =>
                {
                    if (DynamicAdViewGrid.Children.Count > 0)
                    {
                        DynamicAdViewGrid.Children.Clear();
                    }
                    DynamicAdViewGrid.Visibility = Visibility.Collapsed;
                    ___CloseAdsBtn_.Visibility = Visibility.Collapsed;
                    ___ImgBanner_.Visibility = Visibility.Collapsed;
                });
        }
        public void ShowBannerAds()
        {
            LoadBannerDynamically();
        }
        private void AdReceived(object sender, EventArgs e)
        {
            //
            DynamicAdViewGrid.Visibility = Visibility.Visible;
            ___CloseAdsBtn_.Visibility = Visibility.Visible;
            ___ImgBanner_.Visibility = Visibility.Visible;
            //
            IsAdsReceiving = false;
        }
        private void InteractedWithAd(object sender, EventArgs e)
        {
            //
            int tt = 0;
            tt++;
            //
        }

        public bool IsBannerAppearing()
        {
            return (DynamicAdViewGrid.Visibility == Visibility.Visible);
        }
        private void AdCollapsed(object sender, EventArgs e)
        {
            if (DynamicAdViewGrid.Children.Count > 0)
            {
                DynamicAdViewGrid.Children.Clear();
            }
            DynamicAdViewGrid.Visibility = Visibility.Collapsed;
            ___CloseAdsBtn_.Visibility = Visibility.Collapsed;
            ___ImgBanner_.Visibility = Visibility.Collapsed;
        }

        private void ___CloseAdsBtn__Click(object sender, RoutedEventArgs e)
        {
            HideAdsBanner();
        }

        #endregion
    }

    class AssociationUriMapper : UriMapperBase
    {
        private string tempUri;
        static readonly Regex s_protocolRegex = new Regex(@"^\/Protocol\?encodedLaunchUri=(.+)$");

        static Uri s_lastProtocolUri;
        static Uri s_lastRedirectedUri;
        public override Uri MapUri(Uri uri)
        {
            Match protocolMatch = s_protocolRegex.Match(uri.ToString());
            if (protocolMatch.Success && AppAuthenticationHelper.IsFacebookLoginResponse(uri))
            {
                if (null != s_lastProtocolUri &&
                    s_lastProtocolUri.Equals(uri))
                {
                    // There's a WP8 API bug that calls MapUri twice when activated via protocol
                    // and this check is used to prevent parsing and acting twice
                    return s_lastRedirectedUri;
                }

                s_lastProtocolUri = uri;
                string encodedLaunchUri = HttpUtility.UrlDecode(protocolMatch.Groups[1].Value);
                Uri launchUri = new Uri(encodedLaunchUri);

                // call method to determine where to go / what to do
                s_lastRedirectedUri = new Uri("/ConnectPage.xaml", UriKind.Relative);
                //save the uri
                CCFacebookUtils.FacebookClient.Instance.UriReponse = launchUri;
                //
                return s_lastRedirectedUri;             
            }

            // by default, navigate to the requested uri
            return uri;
        }

    }
}