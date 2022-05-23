import pandas as pd
import requests
import json
import os

def create_tickets (filepath):
    df = pd.read_csv(filepath)
    df.columns = map(str.lower, df.columns)
    df.drop('follow up call', axis=1, inplace=True) # drops a column that was not needed that was causing some issues in formatting
    df = df.dropna(how='all') # Drop empty rows
    df = df.fillna("N/A")
    tickets_dict = {'tickets': []}

    for index, ticket_df in df.iterrows():

        ticket_body = f"""**Employment status**: {ticket_df['employment status'].strip()}

        **Project**: {str(ticket_df['project']).strip()}

        **Location**: {str(ticket_df['location']).strip()}

        **On-site/remote**: {str(ticket_df['on-site/remote']).strip()}

        **Employee Name**: {str(ticket_df['first name']).strip()} {str(ticket_df['last name']).strip()}

        **Email address**: {str(ticket_df['email address']).strip()}

        **Temporary Email password**: {str(ticket_df['email pw']).strip()}

        **Genesys account**: {str(ticket_df['genesys status']).strip()}
        
        **Welcome Email**: {str(ticket_df['welcome email']).strip()}

        **Genesys phone**: {str(ticket_df['genesys phone']).strip()}

        **Azure AD account**: {str(ticket_df['azure ad status']).strip()}
        
        **Temporary Azure AD password**: {str(ticket_df['azure ad pw']).strip()}

        **Workspace access**: {str(ticket_df['ws1 access']).strip()}

        **Workspace enrollment**: {str(ticket_df['ws1 enroll']).strip()}

        **Domain Account**: {str(ticket_df['domain account']).strip()}

        **VPN Access**: {str(ticket_df['vpn access']).strip()}

        **Physical/Building Access**: {str(int(ticket_df['building access'])).strip() if type(ticket_df['building access']) == float else str(ticket_df['building access']).strip()}

        **Accommodations**: {str(ticket_df['accommodations']).strip()}

        **Equipment checked out and sent**: {str(ticket_df['equipment sent']).strip()}

        **Equipment tracking**: {str(ticket_df['tracking']).strip()}

        **Equipment delivery**: {str(ticket_df['delivery status']).strip()}"""

        ticket_body = ticket_body.replace("    ", "")

        subject = f"{ticket_df['start date'].replace('/', '.').strip()} {ticket_df['project'].strip()} Class - {ticket_df['first name'].strip()} {ticket_df['last name'].strip()}"
        tickets_dict['tickets'].append({'subject': subject, 'comment': {'body': ticket_body}})

    payload = json.dumps(tickets_dict)

    # Set the request parameters
    domain = "test"
    url = f'https://{domain}.zendesk.com/api/v2/tickets/create_many.json'
    user = 'c.phillips'
    pwd = 'password1'
    headers = {'content-type': 'application/json'}

    # Do the HTTP post request
    response = requests.post(url, data=payload, auth=(user, pwd), headers=headers)

    # Check for HTTP codes other than 201 and 200 (Created)
    if response.status_code != 201 and response.status_code != 200:
        print('Status:', response.status_code, "Problem with the request. Please press 'Enter' to try again.")
        input()

    # Report success
    print("Successfully created the tickets. Press 'Enter' to return to the menu.")
    input()


if __name__ == "__main__":
    while True:
        # Clear system screen
        os.system("cls")
        # Select mode
        mode = input("Menu: \n\nPlease enter the corresponding number:\n1. Submit tickets using CSV file\n2. Close program\n\nSelection: ").strip()
        if mode != '1' and mode != '2':
            input("Type either '1' or '2.' Please press 'Enter' and try again.\n")
        elif mode == '1':
            filepath = input("CSV file path (e.g. \"C:/Files/data.csv)\": ").replace("\"", "").replace("\'", "").strip()
            # Check if file exists
            if os.path.exists(filepath):
                # Check if file is csv
                if filepath.endswith(".csv"):
                    try:
                        create_tickets(filepath)
                    except Exception as e:
                        input(f"An error occured in the following area: {e}. Please press 'Enter' and try again.")
                else:
                    input(f"Please use a CSV file, not a {filepath[filepath.index('.')+1:].upper()}. Please press 'Enter' and try again.")
            else:
                input ("No file exists. Please press 'Enter' and try again.")
                continue
        elif mode == '2':
            break
