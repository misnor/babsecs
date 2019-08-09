#include "doctest.h"
#include "PubSub.hpp"

#include <functional>
#include <iostream>

struct ExampleEvent
{
	int payload;

	ExampleEvent() : payload(0) {}
	ExampleEvent(int payload) : payload(payload) {}
};

struct ExampleObserver {
	int eventCount = 0;
	int lastPayloadReceived = 0;

	void HandlExample(const ExampleEvent& e)
	{
		this->lastPayloadReceived = e.payload;
		eventCount++;
	}
};


TEST_SUITE("Event Manager")
{
	EventManager eventManager;
	int expectedPayload = 111;

	TEST_CASE("EventManager can subscribe and broadcast and event to an anonymous function")
	{
        eventManager.Subscribe<ExampleEvent>([&](const ExampleEvent& e) {
            REQUIRE(e.payload == expectedPayload);
        });
		eventManager.Broadcast<ExampleEvent>(ExampleEvent(expectedPayload));
	}

	TEST_CASE("EventManager can subscribe and broadcast to a function in a class")
	{
		ExampleObserver observer;
		REQUIRE(observer.eventCount == 0);

		eventManager.Subscribe<ExampleEvent>(std::bind(&ExampleObserver::HandlExample, &observer, std::placeholders::_1));
		eventManager.Broadcast<ExampleEvent>(ExampleEvent(expectedPayload));

		REQUIRE(observer.eventCount == 1);
		REQUIRE(observer.lastPayloadReceived == expectedPayload);
	}

	TEST_CASE("EventManager can broadcast an event no one is listening to")
	{
		eventManager.Broadcast<ExampleEvent>(ExampleEvent(expectedPayload));
	}
}