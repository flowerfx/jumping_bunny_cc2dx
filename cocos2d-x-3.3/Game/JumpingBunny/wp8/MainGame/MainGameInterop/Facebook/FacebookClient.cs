using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.IO.IsolatedStorage;
using Facebook;
using Facebook.Client;
namespace CCFacebookUtils
{
    public class FacebookClient
    {
        private static FacebookClient instance;
        private string accessToken;
        private Uri p_uriReponse;

        public static String appId = "577579969046252";
        public static String clientSecret = "7bbfa5843bbdfeb1450c36e832a16bd9";
        public static String scope = "publish_actions";
        public static String ProductID = "092498bc621a4c68a9ccf59d00988687";

        private Facebook.FacebookClient fbclient;
        private string p_id = "";
        private string p_NameFB = "";
        private string p_EmailFB = "";
        private string p_Score = "";
        private Dictionary<String, String> p_listFriendScore;
        public FacebookClient()
        {
            IsolatedStorageSettings appSettings = IsolatedStorageSettings.ApplicationSettings;
            try
            {
                accessToken = (string)appSettings["accessToken"];
            }
            catch (KeyNotFoundException e)
            {
                accessToken = "";
            }
        }

        public static FacebookClient Instance
        {
            get
            {
                if (instance == null)
                    instance = new FacebookClient();
                return instance;
            }
            set
            {
                instance = value;
            }
        }

        public string AccessToken
        {
            get
            {
                return accessToken;
            }
            set
            {
                accessToken = value;
                IsolatedStorageSettings appSettings = IsolatedStorageSettings.ApplicationSettings;
                if (accessToken.Equals(""))
                    appSettings.Remove("accessToken");
                else
                {
                    if (!appSettings.Contains("accessToken"))
                        appSettings.Add("accessToken", accessToken);
                    else
                        appSettings["accessToken"] = accessToken;
                }
            }
        }

        public string ID { get { return p_id; } set { p_id = value; } }
        public string NameFB { get { return p_NameFB; } set { p_NameFB = value; } }
        public string EmailFB { get { return p_EmailFB; } set { p_EmailFB = value; } }

        public Uri UriReponse
        {
            get
            {
                return p_uriReponse;
            }
            set
            {
                p_uriReponse = value;
            }
        }

        public virtual String GetLoginUrl()
        {
            return "fbconnect://authorize? client_id=" + appId + "& scope=" + scope + "& redirect_uri=msft-" + ProductID + "://authorize";
        }

        public virtual String GetAccessTokenRequestUrl(string code)
        {
            return "https://graph.facebook.com/oauth/access_token?client_id=" + appId + "&redirect_uri=https://www.facebook.com/connect/login_success.html&client_secret=" + clientSecret + "&code=" + code;
        }

        public virtual String GetAccessTokenExchangeUrl(string accessToken)
        {
            return "https://graph.facebook.com/oauth/access_token?client_id=" + appId + "&client_secret=" + clientSecret + "&grant_type=fb_exchange_token&fb_exchange_token=" + accessToken;
        }

        public void ExchangeAccessToken(UploadStringCompletedEventHandler handler)
        {
            WebClient client = new WebClient();
            client.UploadStringCompleted += handler;
            client.UploadStringAsync(new Uri(GetAccessTokenExchangeUrl(FacebookClient.Instance.AccessToken)), "POST", "");
        }

        public async void RequestUserScore()
        {
            if (AccessToken != null && fbclient!= null)
            {
                try
                {
                    dynamic result = await fbclient.GetTaskAsync("/me/scores");
                    var data = result.data;
                    if (data.Count > 0 && data[0] != null)
                    {
                        int score = (int)data[0].score;
                        p_Score = score.ToString();
                    }
                    else
                    {
                        p_Score = "0";
                    }
                }
                catch (FacebookOAuthException e)
                {
                    string str = e.Message;
                }
            }
        }

        public string GetUserScore()
        {
            return p_Score;
        }

        public async void UploadUserScore(int score)
        {
            if (AccessToken != null && fbclient != null)
            {
                Dictionary<string, object> fbParams = new Dictionary<string, object>();
                fbParams["score"] = score;          
                try
                {
                    await fbclient.PostTaskAsync("/me/scores", fbParams);
                }
                catch (FacebookOAuthException e)
                {

                }
            }
        }

        public async void ShareOnFacebook(string str)
        {
            if (AccessToken != null && fbclient != null)
            {
                var applicationID = Windows.ApplicationModel.Store.CurrentApp.AppId;
                Dictionary<string, object> fbParams = new Dictionary<string, object>();
                fbParams["message"] = str;
                fbParams["link"] = "http://www.windowsphone.com/en-us/store/app/jumping-bunny/" + applicationID.ToString();
                try
                {
                    String cmd = "/me/feed";
                    await fbclient.PostTaskAsync(cmd, fbParams);
                }
                catch (FacebookOAuthException e)
                {

                }
            }
        } 

        public async void GetFacebookProfile(string accessToken)
        {
            if (accessToken != null)
            {
                fbclient = new Facebook.FacebookClient(accessToken);

                //Fetch facebook UserProfile:   
                try
                {
                    dynamic result = await fbclient.GetTaskAsync("me");
                    p_id = result.id;
                    p_EmailFB = result.email;
                    p_NameFB = result.name;
                }
                catch(FacebookOAuthException e)
                {
                    RefreshAccessToken(accessToken); 
                }

                //Format UserProfile:    
            }
        }

        public string GetUserProfilePicture(string UserID, string AccessToken)
        {
            string profilePictureUrl = string.Format("https://graph.facebook.com/{0}/picture?type={1}&access_token={2}", UserID, "square", AccessToken);
            return profilePictureUrl;
        }
        public string GetProfilePictureFromID(string UserID)
        {
            string profilePictureUrl = string.Format("https://graph.facebook.com/{0}/picture?type={1}", UserID, "square");
            return profilePictureUrl;
        }
        public async void RefreshAccessToken(string currentAccessToken)
        {
            Dictionary<string, object> fbParams = new Dictionary<string, object>();
            fbParams["client_id"] = appId;
            fbParams["grant_type"] = "fb_exchange_token";
            fbParams["client_secret"] = clientSecret;
            fbParams["fb_exchange_token"] = currentAccessToken;
            JsonObject publishedResponse = await fbclient.GetTaskAsync("/oauth/access_token", fbParams) as JsonObject;
            AccessToken = publishedResponse["access_token"].ToString();
            //reget the facebook profile
            fbclient = new Facebook.FacebookClient(accessToken);  
            dynamic result = await fbclient.GetTaskAsync("me");
            p_id = result.id;
            p_EmailFB = result.email;
            p_NameFB = result.name;
        }
    }
}
