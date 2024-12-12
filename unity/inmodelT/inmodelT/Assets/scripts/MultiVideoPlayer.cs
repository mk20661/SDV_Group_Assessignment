using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Video;

public class MultiVideoPlayer : MonoBehaviour
{
    public VideoPlayer videoPlayer; // 视频播放器
    public RawImage rawImage;       // 显示视频的 RawImage
    public Button button1;          // 按钮 1
    public Button button2;          // 按钮 2
    public Button closeButton;      // 关闭视频的按钮

    public VideoClip videoClip1;    // 视频 1
    public VideoClip videoClip2;    // 视频 2
    public ParticleSystem snow;
    public ParticleSystem fireWorks;

    public Canvas videoCanvas;

    void Start()
    {
        // 设置 RawImage 的纹理为 VideoPlayer 的 Render Texture
        videoCanvas.enabled = false;
        rawImage.texture = videoPlayer.targetTexture;
        snow.Stop();
        fireWorks.Stop();

        // 绑定按钮事件
        button1.onClick.AddListener(() => PlayVideo(videoClip1,fireWorks));
        button2.onClick.AddListener(() => PlayVideo(videoClip2,snow));
        closeButton.onClick.AddListener(CloseVideo);

        // 添加 RawImage 点击事件监听
        rawImage.gameObject.AddComponent<Button>().onClick.AddListener(TogglePausePlay);
    }

    // 播放视频的方法
    public void PlayVideo(VideoClip videoClip,ParticleSystem effectSystem)
    {
        videoCanvas.enabled = true;
        rawImage.gameObject.SetActive(true);
        if (videoPlayer.isPlaying)
        {
            videoPlayer.Stop(); // 停止当前播放的视频
        }
        effectSystem.Stop();
        videoPlayer.clip = videoClip; // 设置新的视频资源
        videoPlayer.Play();           // 播放视频
    }

    // 切换暂停/播放状态
    public void TogglePausePlay()
    {
        if (videoPlayer.isPlaying)
        {
            videoPlayer.Pause(); // 暂停视频
        }
        else
        {
            videoPlayer.Play();  // 播放视频
        }
    }

    // 关闭视频的方法
    public void CloseVideo()
    {
        videoPlayer.Stop(); // 停止播放
        snow.Stop();
        fireWorks.Stop();
        rawImage.gameObject.SetActive(false); // 隐藏视频显示区域
        videoCanvas.enabled = false;
    }

    public void OpenSnow(){
        snow.Play();
    }

    public void OpenFireworks(){
        fireWorks.Play();
    }
}
