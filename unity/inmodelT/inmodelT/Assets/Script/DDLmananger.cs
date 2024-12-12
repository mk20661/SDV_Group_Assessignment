using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using TMPro;
using UnityEngine.UI;

public class CourseAPIReader : MonoBehaviour
{
    public TextMeshProUGUI course1NameText;
    public TextMeshProUGUI course2NameText;
    public TextMeshProUGUI course3NameText;
    public TextMeshProUGUI course4NameText;
    public TextMeshProUGUI courseDetailsName;
    public TextMeshProUGUI courseDetailDDL;
    public Button course1Button;
    public Button course2Button;
    public Button course3Button;
    public Button course4Button;
    //public Button closeButton;
    public Canvas courseDB;

    private string apiUrl = "http://10.129.111.6:5000/ddl";
    private List<Course> courses;

    void Start()
    {   
        courseDB.enabled = false;
        course1Button.interactable = false;
        course2Button.interactable = false;
        course3Button.interactable = false;
        course4Button.interactable = false;

        course1Button.onClick.AddListener(() => DisplayCourseDetails(0));
        course2Button.onClick.AddListener(() => DisplayCourseDetails(1));
        course3Button.onClick.AddListener(() => DisplayCourseDetails(2));
        course4Button.onClick.AddListener(() => DisplayCourseDetails(3));

        StartCoroutine(FetchCourses());
    }

    IEnumerator FetchCourses()
    {
        using (UnityWebRequest request = UnityWebRequest.Get(apiUrl))
        {
            yield return request.SendWebRequest();

            if (request.result == UnityWebRequest.Result.Success)
            {
                string jsonResponse = request.downloadHandler.text;
                CourseData courseData = JsonUtility.FromJson<CourseData>(jsonResponse);

                if (courseData != null && courseData.courses != null)
                {
                    courses = courseData.courses;
                    UpdateCourseNames();
                }
            }
        }
    }

    void UpdateCourseNames()
    {
        course1NameText.text = courses.Count > 0 ? $"{courses[0].code} {courses[0].name}" : "No data available";
        course2NameText.text = courses.Count > 1 ? $"{courses[1].code} {courses[1].name}" : "No data available";
        course3NameText.text = courses.Count > 2 ? $"{courses[2].code} {courses[2].name}" : "No data available";
        course4NameText.text = courses.Count > 3 ? $"{courses[3].code} {courses[3].name}" : "No data available";

        if (courses.Count > 0) course1Button.interactable = true;
        if (courses.Count > 1) course2Button.interactable = true;
        if (courses.Count > 2) course3Button.interactable = true;
        if (courses.Count > 3) course4Button.interactable = true;
    }


    void DisplayCourseDetails(int index)
    {
        if (courses != null && index < courses.Count)
        {   
            courseDB.enabled = true;
            var course = courses[index];
            string courseName = $"<b>{course.code}</b>\n";
            string DDLdeatials = "Deadline:\n";
            foreach (var deadline in course.deadlines)
            {
                DDLdeatials += $"- {deadline.title}:\n   {deadline.date} at {deadline.time}\n";
            }

            courseDetailsName.text = courseName;
            courseDetailDDL.text = DDLdeatials;
        }
        else
        {
            courseDetailsName.text = "No course details available.";
            courseDetailDDL.text = "No course Deadline";
        }
    }

    [System.Serializable]
    public class Deadline
    {
        public string title;
        public string type;
        public string date;
        public string time;
    }

    [System.Serializable]
    public class Course
    {
        public string name;
        public string code;
        public List<Deadline> deadlines;
    }

    [System.Serializable]
    public class CourseData
    {
        public List<Course> courses;
    }
}
