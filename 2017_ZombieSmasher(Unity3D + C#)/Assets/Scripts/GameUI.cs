using UnityEngine;
using UnityEngine.UI;
using System;
using UnityEngine.Events;

public class GameUI : MonoBehaviour
{
    public CounterComponent _livesComponent;
    public CounterComponent _bombsComponent;
    public Scrollbar _timeProgress;

    public MessageWindow _loseWindow;
    public MessageWindow _winWindow;

    public void UpdateTime(float startTime, int duration)
    {
        float round = (Time.time - startTime) / duration;
        if (round < 1)
        {
            _timeProgress.value = round;
            _timeProgress.GetComponentInChildren<Text>().text = (Math.Ceiling(startTime + duration - Time.time)).ToString();
        }
        else
        {
            _timeProgress.GetComponentInChildren<Text>().text = "0";
        }
    }

    public void UpdateLives(int lives)
    {
        _livesComponent.ChangeText(lives);
    }

    public void UpdateBombs(int bombs)
    {
        _bombsComponent.ChangeText(bombs);
    }

    public void UpdateAll(int lives, int bombs, float startTimeValue, int duration)
    {
        UpdateTime(startTimeValue, duration);
        UpdateLives(lives);
        UpdateBombs(bombs);
    }

    public void ShowWinWindow(Action callback)
    {
        _winWindow.Show(true);
        _winWindow.SetContent("You win", "Continue", callback);        
    }

    public void ShowLoseWindow(Action callback)
    {
        _loseWindow.Show(true);
        _loseWindow.SetContent("You lose", "Restart", callback);
    }

    public void SetBombsClick(UnityAction func)
    {
        Button button = _bombsComponent.GetComponentInChildren<Button>();
        if(button != null)
        {
            button.onClick.AddListener(func);
        }
    }
}
