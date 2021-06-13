import requests
import time
#import playsound
dist = 240
dist_id = ""
dist_name = ""
#dist = input("Enter district ID")
#dhn - 257,ramgarh-254,ranchi-240

#dist = input("Enter district ID")
#date = input("Enter the Date")
date = '14-06-2021'
state_id = 15  
URL = 'https://cdn-api.co-vin.in/api/v2/appointment/sessions/public/findByDistrict?district_id={}&date={}'.format(dist,date)
URL2 = 'https://cdn-api.co-vin.in/api/v2/admin/location/districts/{}'.format(state_id)
header = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.77 Safari/537.36'}

def findistcode():
    print('Listing District ID and District Name')
    result2 = requests.get(URL2,headers=header)
    result2_response = result2.json()
    data2 = result2_response["districts"]
    for each in data2:
        print(each["district_id"])
        dist_id = each["district_id"]
        print(each["district_name"])
        dist_name = each["district_name"]
        if(each["district_name"] == "Dhanbad"):
            print('\nMy city\n')

#findistcode()

def FindAvailblity():
    count = 0
    #dist = input("Enter district ID")
    #date = input("Enter the Date")
    URL = 'https://cdn-api.co-vin.in/api/v2/appointment/sessions/public/findByDistrict?district_id={}&date={}'.format(dist,date)
    result = requests.get(URL,headers=header)
    response_json = result.json()
    data = response_json["sessions"]
    print('Date Entered is {}'.format(date))
    for each in data:
        if((each["available_capacity"] > 0 ) & (each["min_age_limit"] == 18)):
            count += 1
            print('Start*********{}'.format(count))
            print('Slot is available')
            print(each["name"])
            print(each["pincode"])
            print(each["vaccine"])
            print(each["available_capacity"])
            print('End*******{}'.format(count))
        #if(each["vaccine"] == "COVAXIN"):
           # print('Covaxin Available')
    print('\nTotal Center having vaccine is - {}'.format(count))
            
            #playsound('D:\Alarm01.wav')
    return True 
    if(count == 0):
        print("Slot Not available")
        return False

while(FindAvailblity() != True):
    time.sleep(5)
    FindAvailblity






