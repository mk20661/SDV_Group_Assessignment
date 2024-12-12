using System.Collections;
using UnityEngine;
using UnityEngine.Networking;
using TMPro;
using System;

public class ChineseHolidayManager : MonoBehaviour
{
    public TextMeshProUGUI ChineseHolidayName;
    public TextMeshProUGUI ChineseHolidayDate;
    public TextMeshProUGUI ChineseHolidayDayUntil;
    private string apiUrl = "http://10.129.111.6:5000/next-chinese-holiday";
    private bool isFetching = false;

    void Start()
    {
        if (ChineseHolidayName == null)
        {
            ChineseHolidayName = GetComponent<TextMeshProUGUI>();
        }
        if (ChineseHolidayDate == null)
        {
            ChineseHolidayDate = GetComponent<TextMeshProUGUI>();
        }
        if (ChineseHolidayDayUntil == null)
        {
            ChineseHolidayDayUntil = GetComponent<TextMeshProUGUI>();
        }
        InvokeRepeating(nameof(RepeatFetchData), 0f, 10f);
    }

    void RepeatFetchData()
    {
        StartCoroutine(GetCNHolidayData());
    }

    IEnumerator GetCNHolidayData()
    {   
        if (isFetching) yield break;
        isFetching = true;

        using (UnityWebRequest request = UnityWebRequest.Get(apiUrl))
        {
            yield return request.SendWebRequest();
            isFetching = false;

            if (request.result == UnityWebRequest.Result.Success)
            {
                string jsonResponse = request.downloadHandler.text;
                HolidayData holidayData = JsonUtility.FromJson<HolidayData>(jsonResponse);

                UpdateUI(holidayData);
            }
            else
            {
                Debug.LogError("Failed to fetch holiday data: " + request.error);
            }
        }
    }

    void UpdateUI(HolidayData data)
    {
        ChineseHolidayName.text = "Next Chinese Festival:\n" + data.name;
        ChineseHolidayDate.text = "Date: " + DateTime.Parse(data.date.ToString()).ToString("dd-MM-yyyy");;
        ChineseHolidayDayUntil.text = data.days_until.ToString();
    }

    [System.Serializable]
    public class HolidayData
    {
        public string date;
        public int days_until;
        public string name;
    }
}
