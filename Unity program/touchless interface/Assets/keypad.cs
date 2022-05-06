using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class keypad : MonoBehaviour
{
    public Text text;
    public void refNum(int num){
        text.text = text.text + num.ToString();
    }
}
