using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR.ARFoundation;

public class ARImageTargetHandler : MonoBehaviour
{
    [SerializeField]
    private ARTrackedImageManager arTrackedImageManager;

    [SerializeField]
    private GameObject modelPrefab; // 要生成的模型预制件

    private HashSet<Guid> trackedImages = new HashSet<Guid>(); // 用于记录已经处理过的图像

    void OnEnable()
    {
        if (arTrackedImageManager != null)
        {
            arTrackedImageManager.trackedImagesChanged += OnTrackedImagesChanged;
        }
    }

    void OnDisable()
    {
        if (arTrackedImageManager != null)
        {
            arTrackedImageManager.trackedImagesChanged -= OnTrackedImagesChanged;
        }
    }

    private void OnTrackedImagesChanged(ARTrackedImagesChangedEventArgs eventArgs)
    {
        foreach (var addedImage in eventArgs.added)
        {
            HandleTrackedImage(addedImage);
        }

        foreach (var updatedImage in eventArgs.updated)
        {
            HandleTrackedImage(updatedImage);
        }
    }

    private void HandleTrackedImage(ARTrackedImage trackedImage)
    {
        // 如果模型已经生成，直接返回
        if (trackedImages.Contains(trackedImage.referenceImage.guid)) return;

        // 如果检测到目标并且模型尚未生成
        if (trackedImage.trackingState == UnityEngine.XR.ARSubsystems.TrackingState.Tracking)
        {
            // 在 Image Target 位置生成模型
            Instantiate(modelPrefab, trackedImage.transform.position, trackedImage.transform.rotation);

            // 禁用 ARTrackedImage 的检测功能
            trackedImage.gameObject.SetActive(false);

            // 记录已处理的图像
            trackedImages.Add(trackedImage.referenceImage.guid);
        }
    }
}
