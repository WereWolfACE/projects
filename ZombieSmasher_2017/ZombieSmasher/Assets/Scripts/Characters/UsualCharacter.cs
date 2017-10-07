using System;

public class UsualCharacter : BaseCharacter
{
    public override void SetMovementAlgorithm()
    {
        _movement = new DownMovementAlgorithm();
    }

}
