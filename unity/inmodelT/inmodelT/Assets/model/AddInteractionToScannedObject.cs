using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR.ARFoundation;
using UnityEngine.XR.ARSubsystems;
using Lean.Touch;
using Lean.Common;

public class DynamicInteractionBinder : MonoBehaviour
{
    private ARTrackedImageManager trackedImageManager;

    private void Awake()
    {
        trackedImageManager = FindObjectOfType<ARTrackedImageManager>();
    }

    private void OnEnable()
    {
        if (trackedImageManager != null)
        {
            trackedImageManager.trackedImagesChanged += OnTrackedImagesChanged;
        }
    }

    private void OnDisable()
    {
        if (trackedImageManager != null)
        {
            trackedImageManager.trackedImagesChanged -= OnTrackedImagesChanged;
        }
    }

    private void OnTrackedImagesChanged(ARTrackedImagesChangedEventArgs args)
    {
        foreach (var trackedImage in args.added)
        {
            if (trackedImage.transform.childCount > 0)
            {
                var generatedObject = trackedImage.transform.GetChild(0).gameObject;
                AddLeanTouchComponents(generatedObject);
            }
        }
    }

    private void AddLeanTouchComponents(GameObject targetObject)
    {
        if (targetObject.GetComponent<LeanSelectable>() == null)
        {
            targetObject.AddComponent<LeanSelectable>();
        }

        if (targetObject.GetComponent<LeanDragTranslate>() == null)
        {
            var dragTranslate = targetObject.AddComponent<LeanDragTranslate>();
            dragTranslate.Camera = Camera.main;
        }

        if (targetObject.GetComponent<LeanPinchScale>() == null)
        {
            targetObject.AddComponent<LeanPinchScale>();
        }

        if (targetObject.GetComponent<LeanTwistRotateAxis>() == null)
        {
            var twistRotate = targetObject.AddComponent<LeanTwistRotateAxis>();
            twistRotate.Axis = Vector3.up;
        }
    }
}
