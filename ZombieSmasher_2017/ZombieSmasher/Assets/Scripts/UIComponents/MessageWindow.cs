using UnityEngine;
using UnityEngine.UI;
using System;

public class MessageWindow : MonoBehaviour
{
    public Text _text;
    public Button _button;
    public Text _buttonText;

    private Action _callback;

    private void Start()
    {
        _button.onClick.AddListener(OnButtonClick);
    }
    
    public void Show(bool isShow)
    {
        gameObject.SetActive(isShow);
    }

    public void SetContent(string text, string buttonText, Action callback)
    {
        _text.text = text;        
        _buttonText.text = buttonText;
        _callback = callback;        
    }

    private void OnButtonClick()
    {
        Show(false);
        if(_callback != null)
        {
            _callback.Invoke();
        }
    }
}
