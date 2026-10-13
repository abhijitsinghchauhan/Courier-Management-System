# Test Results (15 test cases)

Executed against the compiled program (fresh Data folder per test, input fed from a script). All 15 passed.

| ID | Operation | Input | Expected | Actual output | Status |
|---|---|---|---|---|---|
| T01 | Add customer | Name: Rahul Sharma; Phone: 9876543210 | Customer saved with generated ID 1001 | Customer added successfully. Customer ID = 1001 | Pass |
| T02 | Validation: phone | Phone 12345, then 9876543210 | Error shown, user asked again, record saved only after valid phone | Error: Phone number must be exactly 10 digits. (re-asked) Customer ID = 1001 | Pass |
| T03 | Add courier | Name: Amit Kumar; Phone: 9988776655 | Courier saved with generated ID 2001 | Courier added successfully. Courier ID = 2001 | Pass |
| T04 | Book Standard parcel | Sender 1001; receiver Neha Verma; 2.5 kg; Standard | Parcel 3001 and Delivery 4001 created; charge 50 + 20 x 2.5 = Rs. 100.00 | Parcel ID 3001; Delivery ID 4001; Charge Rs. 100.00 | Pass |
| T05 | Polymorphic charge | Express 2 kg and Fragile 2 kg | Express (100 + 35x2) x 1.15 = Rs. 195.50; Fragile 80 + 25x2 + 150 = Rs. 280.00 | Express Rs. 195.50; Fragile Rs. 280.00 | Pass |
| T06 | Validation: weight | Weight abc, then 75 | abc rejected and re-asked; 75 kg rejected and booking cancelled | Invalid input. Please enter a number. ERROR: Weight must be between 0.1 kg and 50 kg. | Pass |
| T07 | Invalid sender ID | Book parcel with Customer ID 9999 | Error: customer not found | ERROR: Customer ID 9999 not found. | Pass |
| T08 | Assign courier | Parcel 3001; Courier 2001 | Status changes to Assigned | Parcel 3001 assigned to courier Amit Kumar (ID 2001). Status: Assigned | Pass |
| T09 | Illegal status change | Assigned to Delivered (choice 4) | Rejected; next allowed status is Picked Up | ERROR: Invalid transition: Assigned -> Delivered. Next allowed status: Picked Up. | Pass |
| T10 | Status flow | Picked Up, In Transit, Out For Delivery, Delivered | Every step accepted; final status Delivered | Parcel 3001 is now: Delivered | Pass |
| T11 | Track parcel | Parcel 3001 after Picked Up | Shows receiver, charge, courier and status | Receiver Neha Verma; Status Picked Up; Courier Amit Kumar (9988776655) | Pass |
| T12 | Search customer | Name contains: patel | Priya Patel found (case-insensitive) | Customer 1002 Priya Patel; 1 record(s) found. | Pass |
| T13 | Update parcel weight | Parcel 3001: 2.5 kg changed to 4 kg | Charge recalculated: 50 + 20 x 4 = Rs. 130.00 | Parcel updated successfully. Weight 4.00; Charge 130.00 | Pass |
| T14 | Delete rules | Delete customer 1001 (has parcel); then delete parcel 3001 (y) | Customer delete blocked; parcel and delivery deleted after confirmation | ERROR: Customer has parcel records. Parcel and its delivery record deleted. | Pass |
| T15 | File persistence | Add data, save, exit; start program again; display parcels and deliveries | Old records load automatically and are displayed | Loaded 5 record(s): 2 customer(s), 1 courier(s), 1 parcel(s), 1 delivery record(s); parcel 3001 and delivery 4001 shown | Pass |
