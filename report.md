# Report for Project 3

## How does the simulation work?

### Overall idea

For project 3, our group is simulating the Nota Hospital. For this, we have a dataset of doctors, including the time they start and stop work each day and their skill level, and a dataset of patients that will walk in the door to be seen, with a condition level from 1-10.  Patient data is read from a `.csv` dataset into a vector `incoming patients`, while doctors are read from a different `.csv` into `off_duty_doctors`, and also into the doctor B-Tree, `doctor_tree`.

Each time step, doctors check in and out of work based on their shift times. Doctors checking into work are placed in the `available_doctors` priority queue, which is weighted by doctor skill level. A random number of patients walk in the door to the hospital and are added to the `waiting_room` priority queue based on their conditions. From here, doctors and patients are assigned to each other from the top of each of their queues so that the most skilled doctors treat the most hurt patients. When assigned, patients are put into the `patient_tree` B-Tree and both the patient and doctor are updated to show their relation to each other. Patients that have a doctor will start getting better. Every timestep, the patient B-Tree is traversed and patients conditions are decreased relative to their doctor's skill level. 

When a patient gets all better, their doctor is moved back into the `available_doctors` priority queue to get a new patient, and the information linking the two is cleared. In project 4, the patient will be removed from the B-Tree. Similarly, when a doctor clocks out of work, their patient goes back into the `waiting_room` to find a new doctor.

If the hospital runs out of doctors, patients will stay in the `waiting_room` but their `conditon` will get 1 higher, indicating that it's getting worse. Patients in the `waiting_room` with `condition` levels of 10 or 11 are considered "in critical condition," and if they make it to level 12 they die.

### GUI

The GUI displays several useful pieces of information for tracking the workings of the hospital. On the right is a search feature where you can search for doctors and patients by ID number. 

## B-Tree

## Team contributions

### Trey

- B Tree implementation

### Victoria

- Priority queue
- Data sets
- Time logiv
  - New patients
  - Starting doctors


### Daniel

- Visual representation of simulation with SFML
  - Patient/doctor lookup
- UML Class Diagram
- Time logic
  - Doctor / patient assignments
  - Doctor shifts
  - Patient conditions updates
