using UnityEngine;

public class DownMovementAlgorithm : IMovementAlgorithm
{
    public void Execute(GameObject gameObject, float speed)
    {
        gameObject.transform.position += Vector3.down * speed * Time.deltaTime;
    }
}