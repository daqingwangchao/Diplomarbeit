## To test ipv4 wordflow from one pod to another

### create pods 1 and pods 2
``kubectl apply -f test-pods.yaml``

```
MASTER_POD_IP=$(kubectl get pod test-pod-master -o jsonpath='{.status.podIP}')
echo "Test Pod Master IP: $MASTER_POD_IP"

//install the curl on worker node1
kubectl exec test-pod-node1 -- apt-get update
kubectl exec test-pod-node1 -- apt-get install -y curl
kubectl exec test-pod-node1 -- curl -v $MASTER_POD_IP
```

### delete test pods
``kubectl delete -f test-pods.yaml``