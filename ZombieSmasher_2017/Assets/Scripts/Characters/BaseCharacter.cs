using UnityEngine;

public delegate void DieMessage(BaseCharacter character);
public delegate void DestroyMessage(GameObject character);

abstract public class BaseCharacter : MonoBehaviour
{
    public enum States { Run, Die };
    public DieMessage OnDie;
    public DestroyMessage OnDestroy;

    private Animator animator;    

    public bool isDie
    {
        get { return _currentState == States.Die; }        
    }

    public float Speed
    {
        set { _speed = value; }
    }

    protected States _currentState;
    protected float _speed;
    protected IMovementAlgorithm _movement;

    void Awake()
    {
        animator = GetComponentInChildren<Animator>();
    }    

    public void Move()
    {
        if (isDie)
        {
            return;
        }
        if(_movement != null)
        {
            _movement.Execute(gameObject, _speed);
        }
    }
    
    public virtual void Clicked()
    {
        _currentState = States.Die;
        Die();
        if(OnDie != null)
        {
            OnDie.Invoke(this);
        }
    }

    public void Die()
    {
        animator.SetBool("isRun", false);
        animator.SetTrigger("Die");        
    }

    public void Died()
    {
        if (OnDestroy != null)
        {
            OnDestroy.Invoke(gameObject);
        }
    }

    public void Refresh()
    {
        _currentState = States.Run;
        SetMovementAlgorithm();
    }

    public abstract void SetMovementAlgorithm();
}
