uCluster wclus[processors] __attribute__((unused));
uProcessor *wcpus[processors];

// cout << "number of processors in cluster1 = " << Cluster1.getProcessors() <<
// endl;

{
  eventDispatcher = new EventDispatcher(eventFactory, eventBuffers, buffersize);
  vector<TrackingHandler *> trackins;

  for (int i = 0; i < processors; i++) {
    wcpus[i] = new uProcessor(uThisCluster());

    // customBodyHandler = new CustomBodyHandler(bodyContainer);
    /**
 TrackingHandler * tracking = new TrackingHandler(processHandler,
 customBodyHandler, i + 1, eventBuffers[i], coroutineWriters[i] , random,
 Cluster1); trackins.push_back(tracking);
    **/
    trackins.push_back(new TrackingHandler(
        processHandler, new CustomBodyHandler(bodyContainer), i + 1,
        eventBuffers[i], coroutineWriters[i], random, uThisCluster()));
  }
