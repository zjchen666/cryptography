### Andriod Selinux
Unique User IDs (UIDs): Each Android application runs in its own process and is assigned a unique user ID (UID) at the time of installation. This UID is used to isolate the application's data and processes from those of other applications.  
Linux User and Group Model: Android leverages the Linux kernel's user and group model to enforce security boundaries. Each application runs as a separate Linux user, and its files are owned by this user. This ensures that applications cannot access each other's data unless explicitly allowed.  
Application Sandbox: The unique UID and the Linux user model create an application sandbox. This sandboxing mechanism ensures that applications are isolated from each other, preventing unauthorized access to data and resources.  
