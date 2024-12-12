using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;



public class pictureSlide1 : MonoBehaviour
{

    public List<Texture2D> group1; // 第一组图片
    public List<Texture2D> group2; // 第二组图片
    public List<Texture2D> group3; // 第三组图片
    public List<Texture2D> group4; // 第四组图片

    public List<Texture2D> images; // 当前显示的图片列表
    
    public Canvas courseDB;
    public RawImage imageDisplay;

    private int currentIndex = 0; // 当前图片索引


    // Start is called before the first frame update
    void Start()
    {
      // 初始化显示第一组图片
        if (group1.Count > 0)
        {
            LoadImages(group1);
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    // 加载新的图片组到 images 列表
    public void LoadImages(List<Texture2D> newImages)
    {
        if (newImages.Count > 0)
        {
            images = newImages; // 替换当前显示的图片列表
            currentIndex = 0; // 重置当前图片索引
            imageDisplay.texture = images[currentIndex]; // 显示新组的第一张图片
        }
        else
        {
            Debug.LogWarning("The selected image group is empty!");
        }
    }

    public void ShowPreviousImage()
    {
        if (images.Count > 0)
        {
            currentIndex = (currentIndex - 1 + images.Count) % images.Count; // 循环切换
            imageDisplay.texture = images[currentIndex];
        }
    }

    // 显示下一张图片
    public void ShowNextImage()
    {
        if (images.Count > 0)
        {
            currentIndex = (currentIndex + 1) % images.Count; // 循环切换
            imageDisplay.texture = images[currentIndex];
        }
    }


    // 关闭 Canvas
    public void CloseCanvas()
    {
        if (courseDB != null)
        {
            courseDB.enabled = false; // 隐藏 Canvas
        }
    }

    // 切换到第一组图片
    public void SwitchToGroup1()
    {
        LoadImages(group1);
    }

    // 切换到第二组图片
    public void SwitchToGroup2()
    {
        LoadImages(group2);
    }

    // 切换到第三组图片
    public void SwitchToGroup3()
    {
        LoadImages(group3);
    }

    // 切换到第四组图片
    public void SwitchToGroup4()
    {
        LoadImages(group4);
    }


}
