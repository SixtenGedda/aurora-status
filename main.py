import requests

AURORA_API_URL = "https://services.swpc.noaa.gov/products/noaa-planetary-k-index-forecast.json"

def fetch_aurora_data():
    response = requests.get(AURORA_API_URL)
    data = response.json()
    response.close()
    return data

print((fetch_aurora_data()))
    
