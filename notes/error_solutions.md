# "cannot be loaded because running scripts is disabled on this system"
## Error
```
firebase : File C:\Users\quock\AppData\Roaming\npm\firebase.ps1 cannot be loaded because running scripts is disabled on this system. For   
more information, see about_Execution_Policies at https:/go.microsoft.com/fwlink/?LinkID=135170.
At line:1 char:1
+ firebase login
+ ~~~~~~~~
    + CategoryInfo          : SecurityError: (:) [], PSSecurityException
    + FullyQualifiedErrorId : UnauthorizedAccess
```
## Solution 1
```
Set-ExecutionPolicy -ExecutionPolicy Bypass -Scope CurrentUser
```
## Solution 2
Remove ng.ps1(respective ps1 file) from the directory C:\Users< username >\AppData\Roaming\npm\ then try clearing the npm cache at C:\Users< username >\AppData\Roaming\npm-cache..

# "Error: listen EADDRINUSE: address already in use :::3000"
## Solution
`taskkill /f /im node.exe` and restart node server