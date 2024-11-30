from flask import Flask, jsonify, request
import requests
from datetime import datetime

app = Flask(__name__)

# API URL for retrieving UK bank holidays
GOV_UK_URL = "https://www.gov.uk/bank-holidays.json"

@app.route('/next-holiday', methods=['GET'])
def get_next_holiday():
    try:
        # Fetch holiday data from the API
        response = requests.get(GOV_UK_URL)
        data = response.json()

        # Focus on "england-and-wales" region data
        holidays = data["england-and-wales"]["events"]

        # Get the current date and time
        current_date = datetime.now()

        # Find the next upcoming holiday
        next_holiday = None
        days_until = None  # To calculate the number of days until the holiday
        for holiday in holidays:
            holiday_date = datetime.strptime(holiday["date"], "%Y-%m-%d")
            if holiday_date > current_date:
                next_holiday = holiday
                days_until = (holiday_date - current_date).days
                break

        # If a next holiday is found, return its data and days until
        if next_holiday:
            return jsonify({
                "name": next_holiday["title"],
                "date": next_holiday["date"],
                "days_until": days_until
            })

        # If no future holidays are found, return an error message
        return jsonify({"error": "No upcoming holidays found"}), 404

    except Exception as e:
        # Handle any errors that occur during the process
        return jsonify({"error": str(e)}), 500


if __name__ == '__main__':
    # Run the Flask application on host 0.0.0.0 and port 5000
    app.run(host='0.0.0.0', port=5000)
