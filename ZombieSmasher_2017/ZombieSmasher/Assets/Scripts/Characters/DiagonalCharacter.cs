using UnityEngine;

public class DiagonalCharacter : BaseCharacter
{
    public override void SetMovementAlgorithm()
    {
        float rand = Random.Range(0.0f, 1);
        if (rand > 0.5)
        {
            _movement = new LeftRigthMovementAlgorithm();
        }
        else
        {
            _movement = new DownMovementAlgorithm();
        }
    }
}