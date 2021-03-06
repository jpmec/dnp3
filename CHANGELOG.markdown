### 2.0.1-RC3 ###
* Workaround for broken std::chrono::steady_clock impl thanks to Liam MacIsaac.
* Based on user feedback, the .NET outstation API now uses a more intuitive "ChangeSet" to update values in the outstation.
* The .NET "simulator" has been restored with more features surfaced and outstation plugins can now be written to simulate devices.

### 2.0.1-RC2 ###
* DNPManager.AddTCPClient now takes an additional parameter for the local adapter address (0.0.0.0 == all adapters)
* Overloaded CROB constructor allows for non-standard control codes to be specified.
* Fixed a thread-safety issue in the outstation on data update. The API has shifted for updating the outstation database to accomodate this fix.
* Fixed an uninitialized control code variable in the CROB introduced in 2.0.1-RC1.

### 2.0.1-RC1 ###
* Added support for Group50Var4 (Time and Interval) objects. These are static only objects (no events) that can are read/write.
* Added support for Binary/Analog command events to the parser and the master API courtesy of Sid Hoda.
* Added support for the ASSIGN_CLASS function code.
  * Assignment to the outstation db is automatic, but there is a callback on IOutstationApplication for any desired user action
  * The master can assign class automatically during the startup handshaking based on callbacks in IMasterApplication
* Substantially refactored/simplified the master scheduler to make extension easier.
  * Most tasks that are explicitly rejected via IIN bits (e.g. enable/disable unsol) no longer retry on failure
* Added support for discontigous outstation databases based on an earlier contriubtion from Alan Murray
  * Virtual (dnp3) indices are now assigned after creation of the outstation, but before enabling it
  * Note: Discontigous mode must be set in the OutstationConfig in C++, automatically detected in C#
  * Discontiguous mode is slightly less performant on range-based READ's because of O(log(n)) binary search
* Static and event response types can now be configured on a per-point basis
  * In C++ this occurs after creating the outstation instance, but before enabling it.
  * In C#, all values are specified in the DatabaseTemplate
* Fixed a minor conformance issue: Group1Var1 is now promoted to Group1Var2 if the quality != ONLINE
* The are now two methods for monitoring master tasks
  * Callbacks on IMasterApplication for built-in tasks and ones that were created with a userId
  * Optional per-task callback interfaces supplied when the task was created
* Arbitrary master scans (qualifiers 0x06, 0x00/0x01, 0x07/0x08) can now be created in C# using an API contributed by Damon Sutherland
* Added a flag to the outstation that limits the number of queued event checks to 1. This was done for determinism on embedded systems with limited space to queue executor actions.



### 2.0.0 ###
* Fixed an issue in openpal::Synchronized where the stack would deadlock under heavy contention
* Fixed bug where counter 'numTransportErrorRx' was incremented twice
* The log alias in LogRoot is now dynamically sized removed the artificial limitation


### 2.0.0-RC3 ###
* Dynamic memory allocation now allowed in core library during initialization.
  * Removed various redundant static container types in openpal
  * Database and event buffers now create they underlying storage from configuration
  * Hard limits that were in Configure.h are no longer necessary.
* C++/C# master API now receives callback whenever an IIN field is received to allow user layer processing.
* Fixed a bug where periodic taks with period < 0 would rapid-retry instead ()
* Added an embedded demo for the sam3x8E ARM (arduino DUE)
* Parsers are no longer singletons, both groups of functions to reduce SRAM usage on embedded systems.
* ICommandHandler semantics switched back to Select/Operate to more closely match spec. Operate is indistinguisable from DirectOperate at user layer
* Include guards now prepend library name to avoid potential conflicts
* Master can now bind arbitrary 0x06 (all objects) periodic scans
  

### 2.0.0-RC2 ###

Conformance fixes based on testing with 3rd party harness. Only remaining level 2 conformance item is handling broadcast addressing.
  * Outstation now handles limited count (0x07, 0x08) request headers for class polls and specific variation polls.
  * IOutstationApplication can now handle cold/warm restart function codes.

Master measurement API (ISOEHandler) now informs application of qualifer, group variation, and validity of timestamps for each header received.
  
### 2.0.0-RC1 ###

## Refactoring / Improvements  - General ##
* To enable ports to other platforms and MCUs, the 1.1.x library was divided into 4 sub-libraries.
  * openpal - The Platform Abstraction Layer (PAL) provides abtract base classes for key abstractions like IPhysicalLayer, IExecutor, ITimer, etc.
  * opendnp3 - The core dnp3 library. It only depends on openpal and is platform independent C++11.
  * asiopal - Implements the PAL using boost::asio.  This PAL can be used on server platforms like Windows, Linux, & OSX. It only depends on openpal.
  * asiodnp3 - Provides a nice wrapper to opendnp3 for server platforms. Depends on all three libraries.

* Library uses a new parser and generator that vastly improves readability, maintainability, performance, and safety.
* Example applications for Atmel Studio for Arduino boards have been added. They are not yet production ready, but a proof of concept.
* Stack depth has been reduced to better suite embedded systems.
* Core library is now organized by layer making navigation, maintainence, and learning the library easier.
* C++ libraries are now exception-free. They can be compiled with -fno-exceptions.
* IExecutor now uses an "Action0" which is a parameterless functor. Uses type-safe erasure/lambdas combined with static allocation.
* Core library now uses sprintf instead of iostreams. All usage of std::string removed to improve portability.
* Removed some instances of recursion in the link layer CRC checking. The library is now recursion-free.
* The library no longer uses BOOST. Instead it uses the standalone verison (header only) of [ASIO](http://think-async.com/)
* Tests were ported from BOOST_TEST to [Catch](https://github.com/philsquared/Catch). Catch is header only and redistributed with the project.
* Core library no longer uses STL. This makes ports to MCUs much easier.
* Core library no longer uses dynamic memory allocation. Databases and buffers are either statically allocated or injected allowing the user to choose.
* Library no longer uses size_t as this type changes length on 32/64-bit platforms. Explicit types are now used from <cstdint>.
* Application layer objects and most enums are now generated using a companion Scala project in the 'generation' folder.
* Buffer wrappers "WriteBuffer" and "ReadOnlyBuffer" are now passed around internally instead of pairs of uint8_t* and uint32_t.
* Upgraded solution file to VS2013
* x86 and x64 outputs now build side-by-side
* LogEntry class no longer carries timestamp. This allows the timestamp to be set by the specific platform.
* Setpoint/Control terminology changed to AnalogOutput & BinaryOutput to correspond to the specification.
* Libraries no longer have separate src/include directories. This decision was premature.
* Clang is now an officially supported compiler and will also be used for static analysis. Project compiles with Clang 3.4.
* Logging is now explicitly controlled using the bitfields instead of enums. This allows dnp3 specific log levels to be defined without polluting openpal with their definitions.

## External Features ##

* Ticket 56 - Measurement types can now tell the user if the timestamp is valid or not (bool IsTimeValid() const)
* Ticket 43 - Master command callbacks now provide a type that can distinguish between local and remote failure, e.g. when the repsonse times out or the channel is closed.
* Ticket 47 - TCP client can now perform asynchronous DNS host resolution and try mutliple resolved addresses in a single "open" sequence.
* Ticket 56 - C++ channels can also specify a "strategy" for full control over the wait period for retry.
* Ticket 30 - Outstation API now provides control of the NeedTime IIN bit.
* Ticket 29 - Scans are now exposed as part of the master API. Callbacks can be registered to observe success and failure. Class scans may be defined dynamically and performed on demand.
* C++ TCP Client/Server can now configure native socket types with an optional functor. This allows platform-specific customization of things like TCP keep-alives.
* Master applications now specify "ISOEHandler" (Sequence of Events) instead of "IDataObserver". It allows users to discern between static and event types.
* Renamed outstation API "IDataObserver" to the more descriptive "IMeasurementLoader"
* Outstation now uses a simple interface "ITimeWriteHandler" to receive time write requests
* The start of a master or outstation is delayed until a Enable() method is called. Stacks may be dynamically paused/resumed at runtime with the Enable/Disable methods.
* Master now supports additional types:
  * Frozen Counters, Double-bit Binaries, and Octet Strings
* Outstation now supports additional types:
  * Frozen Counters, Double-bit Binaries, and AO/BO status events
* Java Future can now be called with an optional timeout
* Channels now have expoential backoff capability controlled by min/max retry parameters. 
* Each channel can low control the its log level dynamically via the SetLogFilters() method.

## Bug / conformance fixes ##
* Default integrity poll is now a Class 1/2/3/0 scan as per the specification.
* Outstation now reports events in correct order-of-occurence across all seven types it supports.
* Physical layer disconnects now observer the minRetry parameter. This prevents rapid connect/disconnect cycling when a server is closing connections.
* Transport layer can now receive non-fin packets with payload length between 1 and 249. It's very strange for an implementation to do this, but its actually in the spec.
* Count-of-zero headers are now explicitly disallowed and parsing is halted.
* Ticket 57 - When a READ is received while waiting for a confirm, any further retries should be canceled. (also fixed on 1.1.x)
* Ticket 62 - Fixed a bug where the master scheduler could hang if the the channel was closed while waiting for response to a command.

## Removed ##
* VTO "routers" are now longer provided. 0-length strings of all types (Groups 110-113) are now explicitly not allowed.
* ASIOPAL now explicitly uses std::chrono::steady_clock for timers. There is no longer a typedef.
* Library no longer supports 4-octet qualifier codes. All indices used uint16_t as underlying type.
* Support for "delta" values has been removed. New implementations do not need to support this according to the spec.
