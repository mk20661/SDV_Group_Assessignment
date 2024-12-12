using System.Collections;
using UnityEngine;
using UnityEngine.Networking;
using TMPro;
using System.Data;
using System;

public class UKBankHolidayManager : MonoBehaviour
{
    public TextMeshProUGUI UKBankHolidayName; 
    public TextMeshProUGUI UKBankHolidayDate; 
    public TextMeshProUGUI UKBankHolidayDayUntil; 
    private string apiUrl = "http://10.129.111.6:5000/next-holiday"; 
    private bool isFetching = false; 

    void Start()
    {
        // Ensure all UI components are assigned
        if (UKBankHolidayName == null || UKBankHolidayDate == null || UKBankHolidayDayUntil == null)
        {
            Debug.LogError("Not all TextMeshProUGUI references are assigned in the Inspector.");
            return;
        }
        StartCoroutine(FetchDataPeriodically());
    }

    // Coroutine to fetch data periodically
    IEnumerator FetchDataPeriodically()
    {
        while (true)
        {
            // Fetch holiday data f
            yield return FetchUKHolidayData();
            //  10 seconds before repeating
            yield return new WaitForSeconds(10f);
        }
    }

    IEnumerator FetchUKHolidayData()
    {
        if (isFetching) yield break; 
        isFetching = true; 

        Debug.Log("Starting API request...");

        using (UnityWebRequest request = UnityWebRequest.Get(apiUrl))
        {
            yield return request.SendWebRequest(); 
            isFetching = false; 

            if (request.result == UnityWebRequest.Result.Success)
            {
                string jsonResponse = request.downloadHandler.text; 
                Debug.Log("Received JSON response: " + jsonResponse);

                try
                {
                    HolidayData holidayData = JsonUtility.FromJson<HolidayData>(jsonResponse);
                    UpdateUI(holidayData);
                }
                catch (System.Exception e)
                {
                    Debug.LogError("Error parsing JSON: " + e.Message);
                }
            }
            else
            {
                Debug.LogError("Failed to fetch holiday data: " + request.error);
            }
        }
    }

    void UpdateUI(HolidayData data)
    {
        UKBankHolidayName.text = "Next Bank Holiday:\n" + data.name;
        UKBankHolidayDate.text = "Date: " + DateTime.Parse(data.date.ToString()).ToString("dd-MM-yyyy");
        UKBankHolidayDayUntil.text =  data.days_until.ToString();

        Debug.Log($"Updated UI: Name={data.name}, Date={data.date}, Days Until={data.days_until}");
    }

    [System.Serializable]
    public class HolidayData
    {
        public string date; 
        public int days_until; 
        public string name; 
    }
}
