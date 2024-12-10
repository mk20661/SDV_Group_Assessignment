using System.Collections;
using UnityEngine;
using UnityEngine.Networking;
using XCharts; // 引入 XCharts 命名空间

public class RingChartController : MonoBehaviour
{
    public RingChart ringChart; // XCharts 的 RingChart 对象
    public string apiUrl = "http://10.129.111.6:5000/term-week"; // 替换为 Flask API 的地址
    public int maxWeeks = 15; // 一个学期最多的周数

    void Start()
    {
        StartCoroutine(FetchTermWeek());
    }

    IEnumerator FetchTermWeek()
    {
        UnityWebRequest request = UnityWebRequest.Get(apiUrl);
        yield return request.SendWebRequest();

        if (request.result == UnityWebRequest.Result.Success)
        {
            string responseText = request.downloadHandler.text;
            TermWeekResponse data = JsonUtility.FromJson<TermWeekResponse>(responseText);

            if (data != null && data.left_weeks >= 0)
            {
                // 根据剩余周数更新环形图数据
                UpdateRingChart((float)data.left_weeks / maxWeeks);
            }
        }
        else
        {
            Debug.LogError("Failed to fetch term week data: " + request.error);
        }
    }

    void UpdateRingChart(float fillAmount)
    {
        // 更新 XCharts 的 RingChart 数据
        if (ringChart != null)
        {
            ringChart.series.ClearData(); // 清除现有数据
            ringChart.AddData(0, fillAmount); // 填充当前数据
            ringChart.AddData(0, 1 - fillAmount); // 剩余部分
            ringChart.RefreshChart(); // 刷新图表
        }
    }

    [System.Serializable]
    public class TermWeekResponse
    {
        public string term_start;
        public string current_date;
        public int week_number;
        public int term;
        public int left_weeks;
    }
}
