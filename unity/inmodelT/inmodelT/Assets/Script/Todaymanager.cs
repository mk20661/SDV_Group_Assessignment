using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Todaymanager : MonoBehaviour
{
    // Start is called before the first frame update
    public TextMeshProUGUI textPro; // Reference to the TextMeshPro UGUI component

    void Start()
    {
        if (textPro == null)
        {
            // If the TextMeshPro component is not assigned, try to get it from the current GameObject
            textPro = GetComponent<TextMeshProUGUI>();
        }

        // Initialize the text with the current date and time
        UpdateDateTime();

        // Set up a repeated update every second
        InvokeRepeating(nameof(UpdateDateTime), 1f, 1f);
    }

    // Method to update the date and time
    void UpdateDateTime()
    {
        if (textPro != null)
        {
            // Get the current date and time
            System.DateTime now = System.DateTime.Now;

            // Format as "yyyy-MM-dd DayOfWeek HH:mm:ss"
            string currentDateTime = now.ToString("dd-MM-yyyy") + "    " + now.DayOfWeek + "    " + now.ToString("HH:mm:ss");

            // Update the TextMeshPro text
            textPro.text = $"Current Time:    {currentDateTime}";
        }
        else
        {
            Debug.LogError("TextMeshPro component is not assigned!");
        }
    }
}
