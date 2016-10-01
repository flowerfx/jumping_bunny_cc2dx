using System;
using System.Collections.Generic;
using System.Threading.Tasks;

#if DEBUG
using MockIAPLib;
using Store = MockIAPLib;
#else
using Windows.ApplicationModel.Store;
#endif

namespace cocos2d
{
    class StoreManager
    {
        private static StoreManager m_Instance = null;

        public static StoreManager GetInstance()
        {
            if(m_Instance == null)
            {
                m_Instance = new StoreManager();
            }
            return m_Instance;
        }

        public Dictionary<string, string> StoreItems = new Dictionary<string, string>();

        public StoreManager()
        {
            // Populate the store
           // StoreItems.Add("_bunny_small_pack", "bunny_small_pack"); //comsumable
           // StoreItems.Add("_bunny_med_pack", "bunny_med_pack"); //comsumable
          //  StoreItems.Add("_bunny_mega_pack", "bunny_mega_pack"); //comsumable
           // StoreItems.Add("_bunny_remove_ads", "bunny_remove_ads"); //durable
        }
        public void InitStoreItem(String id, String key)
        {
            StoreItems.Add(id, key);
        }
        public async Task<List<string>> GetOwnedItems()
        {
            List<string> items = new List<string>();

            try
            {
                ListingInformation li = await CurrentApp.LoadListingInformationAsync();

                foreach (string key in li.ProductListings.Keys)
                {
                    if (//CurrentApp.LicenseInformation.ProductLicenses.ContainsKey(key) && 
                        (!CurrentApp.LicenseInformation.ProductLicenses[key].IsActive || !CurrentApp.LicenseInformation.ProductLicenses[key].IsConsumable) &&
                        StoreItems.ContainsKey(key))
                    {
                        items.Add(StoreItems[key]);
                    }
                }

                return items;
            }catch(Exception e)
            {
                return items;
            }
        }   

        public void ClearCache()
        {
#if DEBUG
            MockIAP.ClearCache(); 
#endif
        }
    }
}
