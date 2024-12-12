using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class restButton : MonoBehaviour
{
    [SerializeField]
    private Button restartButton; // restButton

    void Start()
    {
        if (restartButton != null)
        {
            restartButton.onClick.AddListener(RestartScene);
        }
    }

    private void OnDestroy()
    {
        if (restartButton != null)
        {
            restartButton.onClick.RemoveListener(RestartScene);
        }
    }

    private void RestartScene()
    {
        string currentSceneName = SceneManager.GetActiveScene().name;
        SceneManager.LoadScene(currentSceneName);
        Debug.Log("Scene restarted.");
    }
}
