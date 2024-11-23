import requests
from datetime import datetime

# API URL
url = "https://www.gov.uk/bank-holidays.json"

# request
response = requests.get(url)

if response.status_code == 200:
    
    data = response.json()

    # current day time
    today = datetime.now().date()

    # england-and-wales holiday
    events = data['england-and-wales']['events']

    
    upcoming_events = []
    for event in events:
        event_date = datetime.strptime(event['date'], '%Y-%m-%d').date()
        if event_date >= today:
            upcoming_events.append((event_date, event))

    # check future holiday
    if upcoming_events:
        # sort date
        upcoming_events.sort(key=lambda x: x[0])

        # get latest holiday 
        next_event_date, next_event = upcoming_events[0]
        delta_days = (next_event_date - today).days

        # format the date display
        formatted_date = next_event_date.strftime('%d/%m/%Y (%A)')

        title = next_event['title']
        notes = next_event['notes']
        bunting = next_event['bunting']

        print(f"Next Holiday: ")
        print(f"Data: {formatted_date}")
        print(f"Holiday Name: {title}")
        print(f"Days until holiday: {delta_days} days")
    else:
        print("No more Holiday。")
else:
    print(f"Request Error, status code: {response.status_code}")
