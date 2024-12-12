using System.Collections;
using UnityEngine;
using UnityEngine.Networking;
using XCharts.Runtime;
using System.Collections.Generic;

public class RingChartAPI_Term3 : MonoBehaviour
{
    public RingChart ringChart;
    private string apiUrl = "http://10.129.111.6:5000/term-week";
    private bool isFetching = false;

    void Start()
    {
        if (ringChart == null)
        {
            ringChart = GetComponent<RingChart>();
        }

        StartCoroutine(FetchDataPeriodically());
    }

    IEnumerator FetchDataPeriodically()
    {
        while (true)
        {
            yield return FetchDataFromAPI();
            yield return new WaitForSeconds(10f);
        }
    }

    IEnumerator FetchDataFromAPI()
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
                Debug.Log("Received JSON: " + jsonResponse);
                ProcessData(jsonResponse);
            }
            else
            {
                Debug.LogError($"API Request Failed: {request.error}");
            }
        }
    }

    void ProcessData(string json)
    {
        // process the json data
        APIResponse response = JsonUtility.FromJson<APIResponse>(json);
        ringChart.series[0].label.formatter = "{c:f0} Weeks";
        if (response != null)
        {   
            int term = response.term;
            int weekNumber = 0; // current week
            int leftWeeks = 0;   // left week
            int totalWeeks = 0;
            if (term == 3){
                weekNumber = response.week_number; // current week
                leftWeeks = response.left_weeks;   // left week
                totalWeeks = weekNumber + leftWeeks;
                if (totalWeeks < 0)
                {
                Debug.LogError("total weeks < 0");
                return;
                }
            } else if(term == 1){
                leftWeeks = 7;
                totalWeeks = 7;
            }else if(term == 2){
                leftWeeks = 7;
                totalWeeks = 7;
            }
            
            List<double> values = new List<double>
            {
                leftWeeks,
                totalWeeks
            };

            ringChart.UpdateData(0, 0, values);

            // refresh
            ringChart.RefreshChart();

        }
        else
        {
            Debug.LogError("API processing fail");
        }
    }

    [System.Serializable]
    public class APIResponse
    {
        public string current_date;
        public int left_weeks;
        public int term;
        public string term_start;
        public int week_number;
    }
}
