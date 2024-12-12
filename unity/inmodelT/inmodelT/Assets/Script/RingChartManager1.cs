using System.Collections;
using UnityEngine;
using UnityEngine.Networking;
using XCharts.Runtime;
using System.Collections.Generic;
public class RingChartAPI_Term1 : MonoBehaviour
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
        try
        {
            APIResponse response = JsonUtility.FromJson<APIResponse>(json);
            ringChart.series[0].label.formatter = "{c:f0} Weeks";
            if (response != null)
            {
                Debug.Log($"Processing API Response: Term={response.term}, Week={response.week_number}");

                List<double> values = new List<double>
                {
                    response.left_weeks,
                    response.week_number + response.left_weeks - 1
                };

                ringChart.UpdateData(0, 0, values);
                ringChart.RefreshChart();
            }
            else
            {
                Debug.LogError("Failed to process API response.");
            }
        }
        catch (System.Exception e)
        {
            Debug.LogError("Error processing data: " + e.Message);
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
