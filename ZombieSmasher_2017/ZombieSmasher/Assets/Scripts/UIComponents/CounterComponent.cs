using UnityEngine;
using UnityEngine.UI;

public class CounterComponent : MonoBehaviour
{
    public Text _text;

    public void ChangeText(int count)
    {
        _text.text = count.ToString();
    }
}
