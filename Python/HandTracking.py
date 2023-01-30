import cv2
import mediapipe as mp
import time
import pyautogui

cap=cv2.VideoCapture(0) #webcam1

mpHands = mp.solutions.hands
hands = mpHands. Hands() #default parameters Hand detection function
mpDraw = mp.solutions.drawing_utils

pTime = 0 # previous time
cTime = 0 # current time
cx_1=0
cy_1=0
while True:
    succes, img=cap.read()
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(imgRGB)
    #print(results.multi_hand_landmarks) #comprueba que tenemos resultados
    if results.multi_hand_landmarks:
        for handLms in results.multi_hand_landmarks:
            for id, lm in enumerate(handLms.landmark):
                #print(id,lm)
                h, w, c = img.shape #dimensiones de la imagen
                cx, cy = int(lm.x*w), int(lm.y*h) #centro de la imagen
                print(id, cx, cy)
                if id==8: # punto de la mano
                    cv2.circle(img, (cx, cy), 15, (255,0,255), cv2.FILLED)
                    if (cx-cx_1)>50 or (cy-cy_1)>50:
                        pyautogui.moveTo(cx*1.9,cy*1.5)
            #mpDraw.draw_landmarks(img, handLms, mpHands.HAND_CONNECTIONS)
            cx_1=cx
            cy_1=cy
# FPS en pantalla
    cTime = time.time() #current time
    fps = 1/(cTime-pTime)
    pTime = cTime

    cv2.putText(img,str(int(fps)),(10,70), cv2.FONT_HERSHEY_PLAIN, 3,
    (255,0,255), 3)
    cv2.imshow("Image",img)
    cv2.waitKey(1)