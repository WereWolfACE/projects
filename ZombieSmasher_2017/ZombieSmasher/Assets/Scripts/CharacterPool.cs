using UnityEngine;
using System.Collections.Generic;
using UnityEditor;

public class CharacterPool : MonoBehaviour
{
    public List<GameObject> _prefabs;
    private List<GameObject> _characters;

    private void Start()
    {
        _characters = new List<GameObject>();
    }

    public GameObject Create(DieMessage onDie, DestroyMessage onDestroy, Vector2 speeds)
    {
        float speed = Random.Range(speeds.x, speeds.y + 1);
        int index = Random.Range(0, 3);
        foreach (GameObject value in _characters)
        {
            if (value.CompareTag(_prefabs[index].tag) && !value.activeSelf)
            {
                value.transform.localScale = _prefabs[index].GetComponent<Transform>().localScale;
                value.SetActive(true);
                value.GetComponent<BaseCharacter>().Speed = speed;
                value.GetComponent<BaseCharacter>().SetMovementAlgorithm();
                value.GetComponent<BaseCharacter>().Refresh();
                return value;
            }
        }

        GameObject item = Instantiate(_prefabs[index]) as GameObject;
        item.GetComponent<BaseCharacter>().Speed = speed;
        item.GetComponent<BaseCharacter>().SetMovementAlgorithm();
        item.GetComponent<BaseCharacter>().OnDie += onDie;
        item.GetComponent<BaseCharacter>().OnDestroy += onDestroy;

        _characters.Add(item);

        return item;
    }

    public void Delete(GameObject item)
    {
        foreach (GameObject value in _characters)
        {
            if (value == item)
            {
                value.SetActive(false);
                break;
            }
        }
    }   

    public List<GameObject> CharactersOnField()
    {
        return _characters.FindAll(x => x.activeSelf);
    }

    public void Restart()
    {        
        foreach (GameObject value in _characters)
        {            
            value.SetActive(false);
        }
    }

}