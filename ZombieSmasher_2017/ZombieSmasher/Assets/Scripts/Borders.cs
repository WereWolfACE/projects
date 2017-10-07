using UnityEngine;
using System.Collections;

public class Borders : MonoBehaviour
{
    public Transform leftBorder;
    public Transform rightBorder;

    public static Borders Instance { get; private set; }

    public void Awake()
    {
        Instance = this;
    }

    public bool IsOutOfRange(float x, float offset)
    {
        return (x - offset) < leftBorder.position.x || (x + offset) > rightBorder.position.x;
    }

}
