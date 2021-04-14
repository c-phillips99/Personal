from bs4 import BeautifulSoup
import requests
import pandas as pd
from tabulate import tabulate

mode = ""

def get_data(home_link, route):
    website = requests.get(home_link + route)
    data_dict = {}
    if website.ok:
        soup = BeautifulSoup(website.content, 'html.parser')
        data = list(map(lambda element: element.string, soup.find(class_ = "historical_data_table table").find_all("td")))

        default_years = 10
        last_recorded_year = int(data[-2])
        start_year = int(data[0])
        end_year = last_recorded_year if last_recorded_year > start_year-default_years else start_year-default_years

        for index in range(0, (start_year-end_year) * 2, 2):
            year = int(data[index])
            if route != "eps-earnings-per-share-diluted":
                num = int(data[index+1][1:].replace(',', '')) if len(data[index+1]) > 1 else 0
            else:
                num = float(data[index+1][1:].replace(',', '')) if len(data[index+1]) > 1 else 0.0
            data_dict.update({year:num})
    else:
        print(f"No response from website with route: {route} ")

    return data_dict
    

def format_data(data):
    revenue, gross_profit, net_income, c_assets, intangible_assets, assets, c_liabilities, debt, liabilities, retained_earnings, equity, operating_cf, property_changes = list(map(lambda dict: list(dict.values()), data))
    years = list(data[0].keys())
    gross_margin = [round(g/r, 4) if r !=0 else 0 for g, r in zip(gross_profit, revenue)]
    net_margin = [round(n/r, 4) if r !=0 else 0 for n, r in zip(net_income, revenue)]
    c_ratio = [round(ca/cl, 4) if cl !=0 else 0 for ca, cl in zip(c_assets, c_liabilities)]
    tangible_equity = [e-ia for e, ia in zip(equity, intangible_assets)]
    tangible_assets = [a-ia for a, ia in zip(assets, intangible_assets)]
    debt_to_tangible_equity = [round(d/te, 4) if te !=0 else 0 for d, te in zip(debt, tangible_equity)]
    rote = [round(n/te, 4) if te !=0 else 0 for n, te in zip(net_income, tangible_equity)]
    rota = [round(n/ta, 4) if ta !=0 else 0 for n, ta in zip(net_income, tangible_assets)]
    free_cf = [o+p for o, p in zip(operating_cf, property_changes)]

    return years, revenue, gross_profit, net_income, c_assets, tangible_assets, c_liabilities, debt, liabilities, retained_earnings, tangible_equity, free_cf, gross_margin, net_margin, c_ratio, debt_to_tangible_equity, rote, rota


def output_data(data, filename):
    years, revenue, gross_profit, net_income, c_assets, tangible_assets, c_liabilities, debt, liabilities, retained_earnings, tangible_equity, free_cf, gross_margin, net_margin, c_ratio, debt_to_tangible_equity, rote, rota = data
    df = pd.DataFrame({"Year":years, "Revenue":revenue, "Gross":gross_profit, "Net":net_income, "Current A":c_assets, "Assets":tangible_assets, "Current L":c_liabilities, "Debt":debt, "Liab":liabilities, "RE":retained_earnings, "Equity":tangible_equity, "Free CF":free_cf, "G-Marg":gross_margin, "N-Marg":net_margin, "C Ratio":c_ratio, "D/E":debt_to_tangible_equity, "ROTE":rote, "ROTA":rota})

    if mode == "1":
        df.to_csv('csvs/'+filename+'.csv', index=False)
        print(f"File created: {filename}.csv")
    elif mode == "2":
        print(tabulate(df, headers = 'keys', tablefmt = 'psql'))

def main(url):
    home_link = url[:-19]
    csv_name = home_link.split("/")[5]
    data = []

    routes = [
        "revenue",
        "gross-profit",
        "net-income",
        "total-current-assets",
        "goodwill-intangible-assets-total",
        "total-assets",
        "total-current-liabilities",
        "long-term-debt",
        "total-liabilities",
        "retained-earnings-accumulated-deficit",
        "total-share-holder-equity",
        "cash-flow-from-operating-activities",
        "net-change-in-property-plant-equipment"
    ]

    for route in routes:
        data_dict = get_data(home_link, route)
        data.append(data_dict)
    clean_data = format_data(data)
    output_data(clean_data, csv_name)


if __name__ == "__main__":
    while True:
        if mode == "":
            mode = input("\nCommands:\n1: Saving Mode\n2: Screening Mode\n")
        
        response = input("\nPlease type a valid Macro Trends URL, type 'set mode' to change modes, or type 'quit' to exit the program: ")
        if "macrotrends.net/stocks/charts/" in response:
            print("Collecting data...")
            main(response)
        elif response.lower() == "quit":
            print("Closing program.")
            break
        elif response.lower() == "set mode":
            mode = input("\nCommands:\n1: Saving Mode\n2: Screening Mode\n")
        else:
            print("Invalid input.")

    
    