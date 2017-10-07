using UnityEngine;
using System.Collections.Generic;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class Game : MonoBehaviour
{
    public int _roundDuration;
    public int _spawnDelay;
    public int _lives;
    public int _bombs;
    public int _minSpeed;
    public int _maxSpeed;
    public float _bombRadius;
    
    public GameUI _ui;
    public CharacterPool _pool;
    public GameObject _spawnPoint;
    public GameObject _finishPoint;
    public GameObject _explosion;
    public Texture2D _defaultCursor;
    public Texture2D _bombCursor;

    private int _currentLives;
    private int _currentBombs;
    private int _charactersKilled;

    private float _startTime;
    private float _lastSpawnTime;
    private Vector2 _speed;

    private bool _isGameEnd;
    private bool _isBombActive;

    void Start()
    {
        Screen.orientation = ScreenOrientation.Portrait;
        _ui.SetBombsClick(OnBombClick);
        Restart();
    }
    
    void Update()
    {
        if (_isGameEnd)
        {
            return;
        }

        CheckTouch();

        _ui.UpdateTime(_startTime, _roundDuration);
        if (_startTime + _roundDuration <= Time.time)
        {
            GameEnd();
            return;
        }

        if (Time.time - _lastSpawnTime >= _spawnDelay)
        {
            Spawn();
        }

        List<GameObject> characters = _pool.CharactersOnField();
        foreach(var item in characters)
        {
            BaseCharacter character = item.GetComponent<BaseCharacter>();
            character.Move();
            if(item.transform.position.y <= _finishPoint.transform.position.y)
            {
                _pool.Delete(item);
                if (!(character is IUntouchable))
                {
                    _currentLives--;
                    _ui.UpdateLives(_currentLives);
                    if (_currentLives <= 0)
                    {
                        GameEnd();
                        return;
                    }
                }
            }
        }
    }

    private void OnBombClick()
    {
        if(_currentBombs <= 0)
        {
            return;
        }
        if (_isBombActive)
        {
            DeactivateBomb();
        }
        else
        {
            ActivateBomb();
        }
    }

    private void ActivateBomb()
    {
        _currentBombs--;
        _ui.UpdateBombs(_currentBombs);
        _isBombActive = true;
        SetCursor(_bombCursor);
    }

    private void DeactivateBomb()
    {
        _currentBombs++;
        _ui.UpdateBombs(_currentBombs);
        _isBombActive = false;
        SetCursor(_defaultCursor);
    }

    private void Spawn()
    {
        GameObject character = _pool.Create(OnClickedCharacter, OnDestroyCharacter, _speed);
        character.transform.position = _spawnPoint.transform.position + Vector3.right * (Random.value - 0.5f) * _spawnPoint.GetComponent<RectTransform>().rect.width;
        _lastSpawnTime = Time.time;
    }

    private void OnClickedCharacter(BaseCharacter character)
    {
        _charactersKilled++;
        if(character is IUntouchable)
        {
            GameEnd(true);
        }
    }

    private void OnDestroyCharacter(GameObject character)
    {
        _pool.Delete(character);
    }

    private void Restart()
    {
        SetCursor(_defaultCursor);
        _pool.Restart();
        _speed = new Vector2(_minSpeed, _maxSpeed);
        _currentBombs = _bombs;
        _currentLives = _lives;
        _charactersKilled = 0;
        _startTime = Time.time;
        _lastSpawnTime = Time.time;
        _ui.UpdateAll(_currentLives, _currentBombs, _startTime, _roundDuration);
        _isGameEnd = false;
        _isBombActive = false;
    }

    private void GameEnd(bool force = false)
    {
        SetCursor(null);
        _isGameEnd = true;
        if(_currentLives > 0 && !force)
        {
            _ui.ShowWinWindow(Restart);
        }
        else
        {
            _ui.ShowLoseWindow(Restart);
        }
    }

    private void CheckTouch()
    {
        if (Input.GetMouseButtonDown(0) && !_isGameEnd)
        {            
            GameObject current = EventSystem.current.currentSelectedGameObject;
            if (current != null)
            {
                Button button = current.GetComponentInChildren<Button>();
                if (button != null && Input.GetMouseButtonUp(0))
                {
                    button.onClick.Invoke();
                }
            }
            else if (_isBombActive)
            {
                Vector3 explosionPosition = Camera.main.ScreenToWorldPoint(Input.mousePosition);
                explosionPosition.z = -1;

                GameObject obj = Instantiate(_explosion, explosionPosition, Quaternion.identity);
                Destroy(obj, obj.GetComponentInChildren<ParticleSystem>().main.duration);

                List<GameObject> characters = _pool.CharactersOnField();
                foreach (var item in characters)
                {
                    if (Vector3.Distance(explosionPosition, item.transform.position) < _bombRadius)
                    {
                        item.GetComponent<BaseCharacter>().Clicked();
                    }
                }
                
                _isBombActive = false;
                SetCursor(_defaultCursor);

            }
            else
            {
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit hit;

                if (Physics.Raycast(ray, out hit))
                {
                    BaseCharacter character = hit.collider.gameObject.GetComponent<BaseCharacter>();
                    if (character != null)
                    {
                        character.Clicked();
                    }
                }
            }            
        }
    }

    private void SetCursor(Texture2D texture)
    {
        if (texture == null)
        {
            Cursor.SetCursor(texture, Vector2.zero, CursorMode.Auto);
        }
        else
        {
            Cursor.SetCursor(texture, new Vector2(texture.width / 2, texture.height / 2), CursorMode.Auto);
        }
    }
}
