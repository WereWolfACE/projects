using UnityEngine;

public class LeftRigthMovementAlgorithm : IMovementAlgorithm
{
    Vector3 _currentDirection;

    public LeftRigthMovementAlgorithm()
    {        
        _currentDirection = (Random.Range(0.0f, 1f) <= 0.5) ? Vector3.left : Vector3.right;
    }

    public void Execute(GameObject gameObject, float speed)
    {
        float rand = Random.Range(0.0f, 1f);        
        if (rand < 0.0005)
        {            
            ChangeDirection();
        }

        Vector3 pos = gameObject.transform.position;

        float x = _currentDirection.x * speed * Time.deltaTime;        
        float y = Vector3.down.y * speed * Time.deltaTime;
        pos.x += x / 3;
        pos.y += y / 2;
        

        if (Borders.Instance.IsOutOfRange(pos.x, gameObject.GetComponent<Collider>().bounds.size.x / 2))
        {
            ChangeDirection();
        }

        gameObject.transform.position = pos;        
    }

    private void ChangeDirection()
    {
        _currentDirection = (_currentDirection == Vector3.left ? Vector3.right : Vector3.left);
    }
}