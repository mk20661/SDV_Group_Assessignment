using UnityEngine;
using UnityEngine.Video;
using UnityEngine.XR.ARFoundation;
using TMPro;

// Script: Detect AR images and display relevant UI and video
public class uiARFindImage : MonoBehaviour
{
    // Video player
    public VideoPlayer videoUX;
    // Video clips
    public VideoClip v_findImage;
    public VideoClip v_imageDetected;
    // Information texts
    public TextMeshProUGUI t_info1;
    public TextMeshProUGUI t_info2;

    // ARTrackedImageManager for image detection
    private ARTrackedImageManager m_ARTrackedImageManager;
    // ARPlaneManager for managing plane visualization
    private ARPlaneManager m_ARPlaneManager;

    private bool isImageDetected = false; // Whether an image has been detected

    private void Awake()
    {
        videoUX.clip = v_findImage; // Set the initial video clip

        m_ARTrackedImageManager = GetComponent<ARTrackedImageManager>();

        m_ARPlaneManager = GetComponent<ARPlaneManager>();

        if (m_ARPlaneManager != null)
        {
            foreach (var plane in m_ARPlaneManager.trackables)
            {
                plane.gameObject.SetActive(false); // Disable plane objects
            }
            m_ARPlaneManager.planesChanged += OnPlanesChanged; // Listen to plane changes
        }

        m_ARTrackedImageManager.trackedImagesChanged += OnTrackedImagesChanged;
    }

    private void OnDestroy()
    {
        // Unsubscribe from events
        if (m_ARPlaneManager != null)
        {
            m_ARPlaneManager.planesChanged -= OnPlanesChanged;
        }
        m_ARTrackedImageManager.trackedImagesChanged -= OnTrackedImagesChanged;
    }

    private void OnPlanesChanged(ARPlanesChangedEventArgs args)
    {
        foreach (var plane in args.added)
        {
            plane.gameObject.SetActive(false);
        }

        foreach (var plane in args.updated)
        {
            plane.gameObject.SetActive(false);
        }
    }

    private void OnTrackedImagesChanged(ARTrackedImagesChangedEventArgs args)
    {
        foreach (var trackedImage in args.added)
        {
            HandleImageFound(trackedImage);
        }

        foreach (var trackedImage in args.updated)
        {
            HandleImageFound(trackedImage);
        }
    }

    private void HandleImageFound(ARTrackedImage trackedImage)
    {
        if (trackedImage.trackingState == UnityEngine.XR.ARSubsystems.TrackingState.Tracking && !isImageDetected)
        {
            isImageDetected = true; // Mark the image as detected

            // Update video and UI when an image is detected
            videoUX.clip = v_imageDetected;
            videoUX.Play();

            t_info1.text = "Image Detected";
            t_info2.text = trackedImage.referenceImage.name;

            Invoke("TurnOffUI", 1.5f); // Turn off after 5 seconds
        }
    }

    private void TurnOffUI()
    {
        videoUX.gameObject.SetActive(false);
        t_info1.gameObject.SetActive(false); 
        t_info2.gameObject.SetActive(false); 
    }
}
